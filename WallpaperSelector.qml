import qs.components
import qs.components.controls
import qs.components.containers
import qs.config
import qs.services
import "modules/launcher"
import Quickshell
import Quickshell.Wayland
import QtQuick

StyledWindow {
    id: root

    visible: true
    name: "launcher"
    screen: Quickshell.screens[0]
    color: "transparent"
    WlrLayershell.layer: WlrLayer.Top
    WlrLayershell.exclusionMode: ExclusionMode.Ignore
    WlrLayershell.keyboardFocus: WlrKeyboardFocus.OnDemand

    anchors.top: true
    anchors.bottom: true
    anchors.left: true
    anchors.right: true

    property PersistentProperties visibilities: PersistentProperties {
        property bool launcher: true
        property bool utilities: false
        property bool sidebar: false
    }

    property QtObject panels: QtObject {
        property QtObject bar: QtObject {
            property int implicitWidth: 0
        }

        property QtObject popouts: QtObject {
            property bool hasCurrent: false
            property int currentCenter: 0
            property int nonAnimHeight: 0
            property int nonAnimWidth: 0
        }

        property QtObject utilities: QtObject {
            property int implicitWidth: 0
        }
    }

    Item {
        id: frame

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: Appearance.padding.large
        implicitWidth: 800
        implicitHeight: 200
    }

        Item {
            id: content

            focus: true
            anchors.fill: frame
            anchors.margins: Appearance.padding.large

        Keys.onLeftPressed: wallpaperList.decrementCurrentIndex()
        Keys.onRightPressed: wallpaperList.incrementCurrentIndex()
        Keys.onEscapePressed: Qt.quit()
        Keys.onReturnPressed: {
            if (wallpaperList.currentItem?.modelData?.path)
                Wallpapers.setWallpaper(wallpaperList.currentItem.modelData.path);
        }

        StyledTextField {
            id: search
            visible: false
            enabled: false
            text: ""
        }

            WallpaperList {
                id: wallpaperList

            anchors.fill: parent
            anchors.margins: Appearance.padding.normal
            search: search
            visibilities: root.visibilities
            panels: root.panels
            content: content
        }
    }
}
