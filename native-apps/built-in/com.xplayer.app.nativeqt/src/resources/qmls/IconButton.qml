
import QtQuick
import QtQuick.Controls

Item {
    id: container

    signal clicked()
    property var iconColor: "black"
    property alias source: icon.source
    property alias pressed: containerArea.pressed
    property string state: enabled ? (pressed ? "pressed" : "default") : "disabled"
    property real pointSize: 80

    width: pointSize
    height: pointSize
    Image {
        id: icon
        source: ""
        width: container.width*70
        height: container.height*70
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        sourceSize.width: 1024
        sourceSize.height: 1024
        // color: "black"
        opacity: 1
        scale: 0.01
    }

    // Text { color:"white" text: icon.status == Image.Ready ? 'Loaded' : (icon.status == Image.Error ? 'Error' : (icon.status == Image.Loading ? 'Loading' : 'Null' ))}
    MouseArea {
        id: containerArea
        anchors.fill: container
        onClicked: container.clicked()
        onPressedChanged: {
            if(pressed) {
                icon.opacity = 0.6
                icon.scale = 0.011
            } else {
                icon.opacity = 1
                icon.scale = 0.01
            }
        }
    }
}