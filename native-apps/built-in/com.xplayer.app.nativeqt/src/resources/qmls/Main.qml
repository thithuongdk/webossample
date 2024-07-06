
import QtQuick
import QtQuick.Window

Window {
    id: mediaWindow
    property var boderColor: "#ABE338"
    property var backGrColor: "#0F1B07"
    property var textColor: "#FFD700"
    property var text2Color: "#F8F8F2"
    visible: true
    width: 1280
    height: 720
    Row {
        MediaPath {
            id: mediapath
            width: 280
            height: 720
            boderColor: mediaWindow.boderColor
            backGrColor: mediaWindow.backGrColor
            textColor: mediaWindow.textColor
            text2Color: mediaWindow.text2Color

        }
        MediaPlayer {
            id: mediaplayer
            width: 1000
            height: 720
            boderColor: mediaWindow.boderColor
            backGrColor: mediaWindow.backGrColor
            textColor: mediaWindow.textColor
            text2Color: mediaWindow.text2Color
        }
    }
}