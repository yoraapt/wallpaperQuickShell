cmake_minimum_required(VERSION 3.19)

project(caelestia-wallpaper-selector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Gui Quick Concurrent Sql Network DBus)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Qalculate IMPORTED_TARGET libqalculate REQUIRED)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")
qt_standard_project_setup(REQUIRES 6.5)

function(add_caelestia_module target uri)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES;INCLUDE_DIRS" ${ARGN})

    qt_add_qml_module(${target}
        URI ${uri}
        VERSION 1.0
        SOURCES ${ARG_SOURCES}
    )

    target_include_directories(${target} PRIVATE ${ARG_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE Qt::Core Qt::Qml ${ARG_LIBRARIES})
endfunction()

set(CAEL_ROOT "${CMAKE_SOURCE_DIR}/plugin-src/Caelestia")

add_caelestia_module(caelestia-root Caelestia
    SOURCES
        "${CAEL_ROOT}/cutils.hpp"
        "${CAEL_ROOT}/cutils.cpp"
        "${CAEL_ROOT}/qalculator.hpp"
        "${CAEL_ROOT}/qalculator.cpp"
        "${CAEL_ROOT}/appdb.hpp"
        "${CAEL_ROOT}/appdb.cpp"
        "${CAEL_ROOT}/requests.hpp"
        "${CAEL_ROOT}/requests.cpp"
        "${CAEL_ROOT}/toaster.hpp"
        "${CAEL_ROOT}/toaster.cpp"
        "${CAEL_ROOT}/imageanalyser.hpp"
        "${CAEL_ROOT}/imageanalyser.cpp"
    LIBRARIES
        Qt::Gui
        Qt::Quick
        Qt::Concurrent
        Qt::Sql
        PkgConfig::Qalculate
    INCLUDE_DIRS
        "${CAEL_ROOT}"
)

add_caelestia_module(caelestia-internal Caelestia.Internal
    SOURCES
        "${CAEL_ROOT}/Internal/cachingimagemanager.hpp"
        "${CAEL_ROOT}/Internal/cachingimagemanager.cpp"
        "${CAEL_ROOT}/Internal/circularindicatormanager.hpp"
        "${CAEL_ROOT}/Internal/circularindicatormanager.cpp"
        "${CAEL_ROOT}/Internal/hyprdevices.hpp"
        "${CAEL_ROOT}/Internal/hyprdevices.cpp"
        "${CAEL_ROOT}/Internal/hyprextras.hpp"
        "${CAEL_ROOT}/Internal/hyprextras.cpp"
        "${CAEL_ROOT}/Internal/logindmanager.hpp"
        "${CAEL_ROOT}/Internal/logindmanager.cpp"
    LIBRARIES
        Qt::Gui
        Qt::Quick
        Qt::Concurrent
        Qt::Network
        Qt::DBus
    INCLUDE_DIRS
        "${CAEL_ROOT}/Internal"
)

add_caelestia_module(caelestia-models Caelestia.Models
    SOURCES
        "${CAEL_ROOT}/Models/filesystemmodel.hpp"
        "${CAEL_ROOT}/Models/filesystemmodel.cpp"
    LIBRARIES
        Qt::Gui
        Qt::Concurrent
    INCLUDE_DIRS
        "${CAEL_ROOT}/Models"
)
