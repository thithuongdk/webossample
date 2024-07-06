
import QtQuick
import QtQuick.Controls

Item {
    id: container

    signal clicked()
    // property var iconColor: "#FFFFFF"
    property alias iconColor: iconText.color
    property alias bgColor: recBg.color
    property alias source: iconText.text
    property alias pressed: containerArea.pressed
    property string state: enabled ? (pressed ? "pressed" : "default") : "disabled"
    property alias pointSize: container.width
    property alias radius: recBg.radius

    width: 80
    height: pointSize

    Rectangle {
        id: recBg
        width: pointSize
        height: pointSize
        radius: pointSize/2
        // border.color: "#00000000"
        // border.color: "black"
        // border.width: 5
        color: "green"
        opacity: containerArea.pressed?1:0.8
    }
    Text {
        id: iconText
        // anchors.centerIn: recBg
        anchors.horizontalCenter: recBg.horizontalCenter
        // anchors.verticalCenter: recBg.verticalCenter
        y: containerArea.pressed?(-pointSize*0.2):(-pointSize*0.15)
        text: ""
        font.bold: containerArea.pressed
        font.pixelSize: containerArea.pressed?pointSize:(pointSize*0.9)
        color: "black"
    }


    // Image {
    //     id: iconBg
    //     source: ""
    //     width: container.width*70
    //     height: container.height*70
    //     anchors.centerIn: parent
    //     fillMode: Image.PreserveAspectFit
    //     sourceSize.width: 1024
    //     sourceSize.height: 1024
    //     // color: "black"
    //     opacity: 1
    //     scale: 0.01
    //     smooth: true
    //     mipmap: true
    // }
    MouseArea {
        id: containerArea
        anchors.fill: container
        onClicked: container.clicked()
        // onPressedChanged: {
        //     if(pressed) {
        //         icon.opacity = 0.6
        //         icon.scale = 0.011
        //     } else {
        //         icon.opacity = 1
        //         icon.scale = 0.01
        //     }
        // }
    }
}