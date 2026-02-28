/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<cachingimagemanager.hpp>)
#  include <cachingimagemanager.hpp>
#endif
#if __has_include(<circularindicatormanager.hpp>)
#  include <circularindicatormanager.hpp>
#endif
#if __has_include(<hyprdevices.hpp>)
#  include <hyprdevices.hpp>
#endif
#if __has_include(<hyprextras.hpp>)
#  include <hyprextras.hpp>
#endif
#if __has_include(<logindmanager.hpp>)
#  include <logindmanager.hpp>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Caelestia_Internal()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<caelestia::internal::CachingImageManager>("Caelestia.Internal", 1);
    qmlRegisterTypesAndRevisions<caelestia::internal::CircularIndicatorManager>("Caelestia.Internal", 1);
    qmlRegisterEnum<caelestia::internal::CircularIndicatorManager::IndeterminateAnimationType>("caelestia::internal::CircularIndicatorManager::IndeterminateAnimationType");
    qmlRegisterTypesAndRevisions<caelestia::internal::LogindManager>("Caelestia.Internal", 1);
    qmlRegisterTypesAndRevisions<caelestia::internal::hypr::HyprDevices>("Caelestia.Internal", 1);
    qmlRegisterTypesAndRevisions<caelestia::internal::hypr::HyprExtras>("Caelestia.Internal", 1);
    qmlRegisterTypesAndRevisions<caelestia::internal::hypr::HyprKeyboard>("Caelestia.Internal", 1);
    QT_WARNING_POP
    qmlRegisterModule("Caelestia.Internal", 1, 0);
}

static const QQmlModuleRegistration caelestiaInternalRegistration("Caelestia.Internal", qml_register_types_Caelestia_Internal);
