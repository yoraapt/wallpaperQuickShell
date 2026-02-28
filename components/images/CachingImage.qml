import qs.utils
import Caelestia.Internal
import Quickshell
import QtQuick

Image {
    id: root

    property alias path: manager.path
    property alias warmOnly: manager.warmOnly

    // Cached local thumbnails should swap in immediately during PathView animations.
    asynchronous: false
    cache: true
    fillMode: Image.PreserveAspectCrop
    retainWhileLoading: true

    Connections {
        target: QsWindow.window

        function onDevicePixelRatioChanged(): void {
            manager.updateSource();
        }
    }

    CachingImageManager {
        id: manager

        item: root
        cacheDir: Qt.resolvedUrl(Paths.imagecache)
    }
}
