
import QtQuick
import QtQuick.Controls

Item {
    id: container

    signal clicked()
    property alias iconColor: iconText.color
    property bool enableBg: true
    property string bgColor: "#010101"
    property alias source: iconText.text
    property alias pressed: containerArea.pressed
    property string state: enabled ? (pressed ? "pressed" : "default") : "disabled"
    property alias pointSize: container.width
    property alias radius: recBg.radius

    width: 80
    height: pointSize

    Rectangle {
        id: recBg
        visible: enableBg
        width: pointSize
        height: pointSize
        radius: pointSize/2
        color: containerArea.pressed?"#D0D0D0":"#E0E0E0"
        opacity: containerArea.pressed?1:0.8
        border.width: containerArea.pressed?3:2
        border.color: !containerArea.pressed?"#D0D0D0":"#E0E0E0"
    }
    Text {
        id: iconText
        anchors.horizontalCenter: recBg.horizontalCenter
        y: containerArea.pressed?(-pointSize*0.20):(-pointSize*0.18)
        opacity: containerArea.pressed?1:0.8
        text: ""
        font.bold: containerArea.pressed
        font.pixelSize: pointSize*0.9
        color: "black"
    }

    MouseArea {
        id: containerArea
        anchors.fill: container
        onClicked: container.clicked()
    }
}
