
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
            width: mediaPath.width
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
                source: "🗁"
                enableBg: false
                iconColor: mediaPlayer.iconColor
                bgColor: "transparent"
                onClicked: folderDialog.open()
            }
            Item {
                x: 35
                anchors.verticalCenter: parent.verticalCenter
                id: openFolderTextItem
                width: parent.width-45
                height: parent.height-10
                clip: true
                Text {
                    y: 3
                    id: openFolderText
                    text: playerService.folderPath
                    font.pixelSize: 20
                    font.bold: true
                    color: mediaPath.textColor
                }
                SequentialAnimation {
                    id: textFolderAnimation
                    running: openFolderTextItem.width < openFolderText.implicitWidth
                    
                    PauseAnimation {
                        duration: 1000
                    }
                    
                    NumberAnimation {
                        target: openFolderText
                        property: "x"
                        from: openFolderText.x
                        to: -openFolderText.width
                        duration: 20*openFolderText.width
                        loops: Animation.Infinite
                        easing.type: Easing.Linear
                    }
                    onStopped: {
                        openFolderText.x = 0
                    }
                }
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
                            duration: 2000
                        }
                        
                        NumberAnimation {
                            target: textElement
                            property: "x"
                            from: textElement.x
                            to: -textElement.width
                            duration: 20*textElement.width
                            loops: Animation.Infinite
                            easing.type: Easing.Linear
                        }
                        onStopped: {
                            textElement.x = 0
                        }
                    }
                    Row {
                        Text {
                            id: indexElement
                            font.pixelSize: 18
                            font.bold: itemDelegate.ListView.isCurrentItem
                            text: (index+1) + ": "
                            color: itemDelegate.ListView.isCurrentItem?mediaPath.text2Color:mediaPath.textColor
                        }
                        Column {
                            Text {
                                id: textElement
                                font.pixelSize: 18
                                font.bold: itemDelegate.ListView.isCurrentItem
                                text: modelData["title"].toString()
                                color: itemDelegate.ListView.isCurrentItem?mediaPath.text2Color:mediaPath.textColor
                            }
                            Text {
                                x: 20
                                font.pixelSize: 14
                                font.bold: itemDelegate.ListView.isCurrentItem
                                text: "artist: " + modelData["artist"].toString()
                                color: itemDelegate.ListView.isCurrentItem?mediaPath.text2Color:mediaPath.textColor
                            }
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