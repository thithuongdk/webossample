
import QtQuick
import QtQuick.Window

Window {
    id: mediaWindow
    property var boderColor: "#D9D9D9"
    property var backGrColor: "#F4F4F4"
    property var textColor: "#333333"
    property var text2Color: "#1DB954"
    property var iconColor: "#1DB954"
    visible: true
    width: 1280
    height: 720
    color: mediaWindow.backGrColor
    Row {
        MediaPath {
            id: mediapath
            width: 280
            height: 720
            boderColor: mediaWindow.boderColor
            backGrColor: mediaWindow.backGrColor
            textColor: mediaWindow.textColor
            text2Color: mediaWindow.text2Color
            iconColor: mediaWindow.iconColor

        }
        MediaPlayer {
            id: mediaplayer
            width: 1000
            height: 720
            boderColor: mediaWindow.boderColor
            backGrColor: mediaWindow.backGrColor
            textColor: mediaWindow.textColor
            text2Color: mediaWindow.text2Color
            iconColor: mediaWindow.iconColor
        }
    }
}