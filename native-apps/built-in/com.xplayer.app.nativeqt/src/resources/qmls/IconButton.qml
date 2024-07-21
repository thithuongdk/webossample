
import QtQuick
import QtQuick.Controls

Item {
    id: root

    signal clicked()
    property alias pressed: rootArea.pressed
    property string iconColor: "black"
    property bool enableBg: false
    property string bgColor: "#010101"
    property string source: "qrc:/svg/help"
    property string state: enabled ? (root.pressed ? "pressed" : "default") : "disabled"
    property real pointSize: 80
    property real radius: pointSize/2

    width: pointSize
    height: pointSize

    Rectangle {
        id: recBg
        // visible: root.enableBg
        anchors.centerIn: parent
        width: root.pointSize
        height: root.pointSize
        radius: root.radius
        // color: root.pressed?"#D0D0D0":"#E0E0E0"
        color: !root.pressed? "#00000000":"#80E0E0E0"
        opacity: 1
        border.width: root.pressed?3:2
        border.color: !root.pressed? "#00000000":"#80D0D0D0"
        Image {
            id: icon
            visible: root.source !== ""
            source: root.source
            width: root.pointSize*70
            height: root.pointSize*70
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            sourceSize.width: 1024
            sourceSize.height: 1024
            // color: "black"
            opacity: root.pressed?root.opacity*0.9:root.opacity
            scale: root.pressed?0.0095:0.01
            smooth: true
            mipmap: true
        }

        MouseArea {
            id: rootArea
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
