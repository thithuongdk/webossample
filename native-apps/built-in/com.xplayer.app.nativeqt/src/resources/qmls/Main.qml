
import QtQuick
import QtQuick.Window

Window {
    id: mediaWindow
    title: "XPlayer"
    property string boderColor: "#D9D9D9"
    property string backGrColor: "#F4F4F4"
    property string textColor: "#333333"
    property string text2Color: "#1DB954"
    property string iconColor: "#1DB954"
    visible: true
    visibility: Window.Maximized
 
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

    MediaWindowBt {
        id: mediaWindowBt
        visible: false
        anchors.right: parent.right
        anchors.top: parent.top
    }
}
