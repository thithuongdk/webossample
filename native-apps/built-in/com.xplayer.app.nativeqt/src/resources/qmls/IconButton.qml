
import QtQuick
import QtQuick.Controls

Item {
    property real buttonSize: 80
    // property var clickAction: funcOnClicked(){}
    property alias source: playbutton.source
    width: buttonSize
    height: buttonSize
    Image {
        id: playbutton
        source: ""
        width: width*0.6
        height: height*0.6
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        opacity: 1
        scale: 1
        // MouseArea {
        //     id: mousearea
        //     anchors.fill: parent
        //     onClicked: funcOnClicked()
        //     onPressedChanged: {
        //         if(pressed) {
        //             opacity = 0.6
        //             scale = 1.1
        //         } else {
        //             opacity = 1
        //             scale = 1
        //         }
        //     }
        // }
    }
}