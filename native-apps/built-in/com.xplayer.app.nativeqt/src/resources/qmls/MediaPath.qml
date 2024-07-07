
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import app.playerservice 1.0
// import libpbnjson 1.0

Rectangle {
    id: mediaPath

    property var boderColor: "#ABE338"
    property var backGrColor: "#0F1B07"
    property var textColor: "#FFD700"
    property var text2Color: "#F8F8F2"
    property var iconColor: "#F8F8F2"

    width: 280
    height: 720
    // color: mediaPath.backGrColor
    border.width: 4
    radius: 8
    border.color: mediaPath.boderColor
    Column {
        Rectangle {
            id: titleFolderDialog
            width: mediaPath.width;
            height: 40
            border.width: 4
            border.color: mediaPath.boderColor
            radius: 8
            FolderDialog {
                id: folderDialog
                currentFolder: "file://" + playerService.folderPath
                onAccepted: {
                    playerService.folderPath = folderDialog.selectedFolder.toString().replace("file://", "")
                }
            }
            IconButton {
                x: 10
                y: 10
                id: openFolderButton
                pointSize: 20
                source: "🇻🇳"
                iconColor: mediaPlayer.iconColor
                bgColor: "transparent"
                onClicked: folderDialog.open()
            }
            Text {
                x: 30
                y: 5
                width: titleFolderDialog.width-x-10
                text: playerService.folderPath
                font.pixelSize: 20
                font.bold: true
                color: mediaPath.textColor
            }
        }

        Rectangle {
            width: mediaPath.width;
            height: mediaPath.height-titleFolderDialog.height
            border.width: 4
            border.color: mediaPath.boderColor
            radius: 8
            ListView {
                id: musicFileListView
                x: 10
                y: 10
                width: parent.width-20
                height: parent.height
                model: playerService.mediaList
                currentIndex: playerService.mediaIndex
                flickDeceleration: 3000
                boundsBehavior: Flickable.StopAtBounds
                snapMode: ListView.SnapToItem
                clip: true
                delegate: Item {
                    id: itemDelegate
                    height: 45
                    width: parent.width
                    MouseArea {
                        anchors.fill: itemDelegate
                        onClicked: {
                            playerService.mediaIndex = index
                        }
                    }
                    SequentialAnimation {
                        id: textAnimation
                        running: musicFileListView.currentIndex == index && musicFileListView.width < textElement.implicitWidth
                        
                        PauseAnimation {
                            duration: 3000
                        }
                        
                        NumberAnimation {
                            target: textElement
                            property: "x"
                            from: textElement.x
                            to: -textElement.width
                            duration: 10000
                            loops: Animation.Infinite
                            easing.type: Easing.Linear
                        }
                    }
                    Column {
                        Text {
                            id: textElement
                            font.pixelSize: 18
                            font.bold: itemDelegate.ListView.isCurrentItem
                            text: (index+1) + ": " + modelData["title"].toString()
                            color: itemDelegate.ListView.isCurrentItem?mediaPath.text2Color:mediaPath.textColor
                        }
                        Text {
                            font.pixelSize: 14
                            font.bold: itemDelegate.ListView.isCurrentItem
                            text: "artist: " + modelData["artist"].toString()
                            color: itemDelegate.ListView.isCurrentItem?mediaPath.text2Color:mediaPath.textColor
                        }
                    }
                }
            }
            Component.onCompleted: {
                playerService.onMediaListChanged.connect(function(newValue) {
                    musicFileListView.currentIndex = Qt.binding(function() { return playerService.mediaIndex; });
                });
            }
        }
    }
}