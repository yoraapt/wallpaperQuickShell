/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<appdb.hpp>)
#  include <appdb.hpp>
#endif
#if __has_include(<cutils.hpp>)
#  include <cutils.hpp>
#endif
#if __has_include(<imageanalyser.hpp>)
#  include <imageanalyser.hpp>
#endif
#if __has_include(<qalculator.hpp>)
#  include <qalculator.hpp>
#endif
#if __has_include(<requests.hpp>)
#  include <requests.hpp>
#endif
#if __has_include(<toaster.hpp>)
#  include <toaster.hpp>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_Caelestia()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    qmlRegisterTypesAndRevisions<caelestia::AppDb>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::AppEntry>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::CUtils>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::ImageAnalyser>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::Qalculator>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::Requests>("Caelestia", 1);
    qmlRegisterTypesAndRevisions<caelestia::Toast>("Caelestia", 1);
    qmlRegisterEnum<caelestia::Toast::Type>("caelestia::Toast::Type");
    qmlRegisterTypesAndRevisions<caelestia::Toaster>("Caelestia", 1);
    QT_WARNING_POP
    qmlRegisterModule("Caelestia", 1, 0);
}

static const QQmlModuleRegistration caelestiaRegistration("Caelestia", qml_register_types_Caelestia);
