
import QtQuick
import QtQuick.Controls

Item {
    id: container

    signal clicked()
    property alias source: icon.source
    property alias pressed: containerArea.pressed
    property string state: enabled ? (pressed ? "pressed" : "default") : "disabled"
    property real pointSize: 80

    width: pointSize
    height: pointSize
    Image {
        id: icon
        source: ""
        width: container.width*0.7
        height: container.height*0.7
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        opacity: 1
        scale: 1
    }
    MouseArea {
        id: containerArea
        anchors.fill: container
        onClicked: container.clicked()
        onPressedChanged: {
            if(pressed) {
                icon.opacity = 0.6
                icon.scale = 1.1
            } else {
                icon.opacity = 1
                icon.scale = 1
            }
        }
    }
}