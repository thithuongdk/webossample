
import QtQuick
import QtQuick.Window
import app.appservice 1.0

Window {
    id: mediaWindow
    title: "XPlayer"
    property var boderColor: "#D9D9D9"
    property var backGrColor: "#F4F4F4"
    property var textColor: "#333333"
    property var text2Color: "#1DB954"
    property var iconColor: "#1DB954"
    visible: true
    visibility: Window.AutomaticVisibility
    // visible: appService.windowStatus==2 || appService.windowStatus==4
    // visibility: appService.windowStatus==2?Window.Maximized: \
    //             (appService.windowStatus==3?Window.Hidde: \
    //             (appService.windowStatus==4?Window.Minimized: \
    //             Window.AutomaticVisibility:))

// QWindow::Windowed
// QWindow::Minimized
// QWindow::Maximized
// QWindow::FullScreen
// QWindow::AutomaticVisibility
// QWindow::Hidde
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
    Row {
        anchors.right: parent.right
        anchors.top: parent.top
        height: 50
        IconButton {
            id: miniButton
            pointSize: 40
            source: "🗕"
            iconColor: "black"
            bgColor:"transparent"
            onClicked: {
                appService.windowStatus = 3;
            }
        }
        IconButton {
            id: resizeButton
            pointSize: 40
            source: "🗖"
            iconColor: "black"
            bgColor:"transparent"
            onClicked: {
                // if (appService.windowStatus==2) {
                //     appService.windowStatus=4
                // } else if(appService.windowStatus==4) {
                //     appService.windowStatus=2
                // }
            }
        }
        IconButton {
            id: closeButton
            pointSize: 40
            source: "🗙"
            iconColor: "black"
            bgColor:"transparent"
            onClicked: {
                appService.windowStatus = -1;
            }
        }
    }
}