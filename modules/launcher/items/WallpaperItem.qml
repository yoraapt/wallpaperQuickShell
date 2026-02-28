import qs.components
import qs.components.effects
import qs.services
import qs.config
import Caelestia.Models
import Quickshell
import QtQuick

Item {
    id: root

    required property FileSystemEntry modelData
    required property PersistentProperties visibilities

    scale: 0.5
    opacity: 0
    z: PathView.z ?? 0

    Component.onCompleted: {
        scale = Qt.binding(() => PathView.isCurrentItem ? 1 : PathView.onPath ? 0.8 : 0);
        opacity = Qt.binding(() => PathView.onPath ? 1 : 0);
    }

    implicitWidth: image.width + Appearance.padding.normal * 2
    implicitHeight: image.height + Appearance.padding.large * 2

    StateLayer {
        radius: Appearance.rounding.normal

        function onClicked(): void {
            Wallpapers.setWallpaper(root.modelData.path);
            root.visibilities.launcher = false;
        }
    }

    Elevation {
        anchors.fill: image
        radius: image.radius
        opacity: root.PathView.isCurrentItem ? 1 : 0
        level: 4

        Behavior on opacity {
            Anim {}
        }
    }

    StyledClippingRect {
        id: image

        anchors.horizontalCenter: parent.horizontalCenter
        y: Appearance.padding.large
        color: "transparent"
        radius: Appearance.rounding.normal

        implicitWidth: Config.launcher.sizes.wallpaperWidth
        implicitHeight: implicitWidth / 16 * 9

        MaterialIcon {
            anchors.centerIn: parent
            text: "image"
            color: Colours.tPalette.m3outline
            font.pointSize: Appearance.font.size.extraLarge * 2
            font.weight: 600
        }

        Image {
            source: "file://" + root.modelData.path
            asynchronous: true
            retainWhileLoading: true
            cache: false
            fillMode: Image.PreserveAspectCrop
            smooth: !(root.PathView.view?.moving ?? false)
            sourceSize.width: Math.round(width)
            sourceSize.height: Math.round(height)

            anchors.fill: parent
        }
    }

    Behavior on scale {
        Anim {}
    }

    Behavior on opacity {
        Anim {}
    }
}
