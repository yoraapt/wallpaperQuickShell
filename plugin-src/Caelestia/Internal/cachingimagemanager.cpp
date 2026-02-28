#include "cachingimagemanager.hpp"

#include <QtQuick/qquickwindow.h>
#include <qcryptographichash.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qimagereader.h>
#include <qmutex.h>
#include <qpainter.h>
#include <qset.h>
#include <qthreadpool.h>

namespace {

QMutex s_pendingCacheMutex;
QSet<QString> s_pendingCaches;

}

namespace caelestia::internal {

qreal CachingImageManager::effectiveScale() const {
    // Use logical item size for cache keys so warmed thumbnails match what the
    // launcher requests, regardless of output scale or fractional DPI.
    return 1.0;
}

QSize CachingImageManager::effectiveSize() const {
    if (!m_item) {
        return QSize();
    }

    const qreal scale = effectiveScale();
    const QSize size = QSizeF(m_item->width() * scale, m_item->height() * scale).toSize();
    m_item->setProperty("sourceSize", size);
    return size;
}

QQuickItem* CachingImageManager::item() const {
    return m_item;
}

void CachingImageManager::setItem(QQuickItem* item) {
    if (m_item == item) {
        return;
    }

    if (m_widthConn) {
        disconnect(m_widthConn);
    }
    if (m_heightConn) {
        disconnect(m_heightConn);
    }

    m_item = item;
    emit itemChanged();

    if (item) {
        m_widthConn = connect(item, &QQuickItem::widthChanged, this, [this]() {
            updateSource();
        });
        m_heightConn = connect(item, &QQuickItem::heightChanged, this, [this]() {
            updateSource();
        });
        updateSource();
    }
}

QUrl CachingImageManager::cacheDir() const {
    return m_cacheDir;
}

void CachingImageManager::setCacheDir(const QUrl& cacheDir) {
    if (m_cacheDir == cacheDir) {
        return;
    }

    m_cacheDir = cacheDir;
    if (!m_cacheDir.path().endsWith("/")) {
        m_cacheDir.setPath(m_cacheDir.path() + "/");
    }
    emit cacheDirChanged();
}

QString CachingImageManager::path() const {
    return m_path;
}

void CachingImageManager::setPath(const QString& path) {
    if (m_path == path) {
        return;
    }

    m_path = path;
    emit pathChanged();

    if (!path.isEmpty()) {
        updateSource(path);
    }
}

void CachingImageManager::updateSource() {
    updateSource(m_path);
}

void CachingImageManager::updateSource(const QString& path) {
    if (path.isEmpty() || path == m_shaPath) {
        // Path is empty or already updating for this path
        return;
    }

    m_shaPath = path;
    const QString key = cacheKey(path);
    const QSize size = effectiveSize();

    if (key.isEmpty() || !m_item || !size.width() || !size.height()) {
        if (m_shaPath == path) {
            m_shaPath = QString();
        }
        return;
    }

    const QString fillMode = m_item->property("fillMode").toString();
    // clang-format off
    const QString filename = QString("%1@%2x%3-%4.png")
        .arg(key).arg(size.width()).arg(size.height())
        .arg(fillMode == "PreserveAspectCrop" ? "crop" : fillMode == "PreserveAspectFit" ? "fit" : "stretch");
    // clang-format on

    const QUrl cache = m_cacheDir.resolved(QUrl(filename));
    if (m_cachePath != cache) {
        m_cachePath = cache;
        emit cachePathChanged();
    }

    if (!cache.isLocalFile()) {
        qWarning() << "CachingImageManager::updateSource: cachePath" << cache << "is not a local file";
        if (m_shaPath == path) {
            m_shaPath = QString();
        }
        return;
    }

    const QImageReader reader(cache.toLocalFile());
    if (reader.canRead()) {
        if (!m_warmOnly) {
            m_item->setProperty("source", cache);
        }
    } else {
        if (!m_warmOnly) {
            m_item->setProperty("source", QUrl::fromLocalFile(path));
        }
        createCache(path, cache.toLocalFile(), fillMode, size);
    }

    if (m_shaPath == path) {
        m_shaPath = QString();
    }
}

QUrl CachingImageManager::cachePath() const {
    return m_cachePath;
}

bool CachingImageManager::warmOnly() const {
    return m_warmOnly;
}

void CachingImageManager::setWarmOnly(bool warmOnly) {
    if (m_warmOnly == warmOnly) {
        return;
    }

    m_warmOnly = warmOnly;
    emit warmOnlyChanged();
    updateSource();
}

void CachingImageManager::createCache(
    const QString& path, const QString& cache, const QString& fillMode, const QSize& size) const {
    {
        QMutexLocker locker(&s_pendingCacheMutex);
        if (s_pendingCaches.contains(cache)) {
            return;
        }
        s_pendingCaches.insert(cache);
    }

    QThreadPool::globalInstance()->start([path, cache, fillMode, size] {
        auto releasePending = [&cache]() {
            QMutexLocker locker(&s_pendingCacheMutex);
            s_pendingCaches.remove(cache);
        };

        QImage image(path);

        if (image.isNull()) {
            qWarning() << "CachingImageManager::createCache: failed to read" << path;
            releasePending();
            return;
        }

        image.convertTo(QImage::Format_ARGB32);

        if (fillMode == "PreserveAspectCrop") {
            image = image.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        } else if (fillMode == "PreserveAspectFit") {
            image = image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        } else {
            image = image.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }

        if (fillMode == "PreserveAspectCrop" || fillMode == "PreserveAspectFit") {
            QImage canvas(size, QImage::Format_ARGB32);
            canvas.fill(Qt::transparent);

            QPainter painter(&canvas);
            painter.drawImage((size.width() - image.width()) / 2, (size.height() - image.height()) / 2, image);
            painter.end();

            image = canvas;
        }

        const QString parent = QFileInfo(cache).absolutePath();
        if (!QDir().mkpath(parent) || !image.save(cache)) {
            qWarning() << "CachingImageManager::createCache: failed to save to" << cache;
        }

        releasePending();
    });
}

QString CachingImageManager::cacheKey(const QString& path) {
    const QFileInfo info(path);
    if (!info.exists() || !info.isFile()) {
        qWarning() << "CachingImageManager::cacheKey: failed to stat" << path;
        return "";
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);
    const QString canonicalPath = info.canonicalFilePath().isEmpty() ? info.absoluteFilePath() : info.canonicalFilePath();
    const QString metadata = QString("%1:%2:%3")
                                 .arg(canonicalPath)
                                 .arg(info.size())
                                 .arg(info.lastModified(QTimeZone::UTC).toMSecsSinceEpoch());
    hash.addData(metadata.toUtf8());

    return hash.result().toHex();
}

} // namespace caelestia::internal
