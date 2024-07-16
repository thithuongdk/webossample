
import QtQuick
import app.appservice 1.0

Item {
    id: mediaWindowBt
    height: 50
    width: 100
    Row {
        height: parent.height
        IconButton {
            id: miniButton
            pointSize: 30
            source: "🗕"
            iconColor: "black"
            enableBg: false
            onClicked: {
                appService.windowStatus = 3;
            }
        }
        IconButton {
            id: resizeButton
            pointSize: 30
            source: "🗖"
            iconColor: "black"
            enableBg: false
            onClicked: {
                if (appService.windowStatus==2) {
                    appService.windowStatus=4
                } else if(appService.windowStatus==4) {
                    appService.windowStatus=2
                }
            }
        }
        IconButton {
            id: closeButton
            pointSize: 30
            source: "🗙"
            iconColor: "black"
            enableBg: false
            onClicked: {
                appService.windowStatus = -1;
            }
        }
    }
}