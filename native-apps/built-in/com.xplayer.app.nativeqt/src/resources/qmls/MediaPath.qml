
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import app.playerservice 1.0
// import libpbnjson 1.0

Rectangle {
    id: root

    property string boderColor: "#ABE338"
    property string backGrColor: "#0F1B07"
    property string textColor: "#FFD700"
    property string text2Color: "#F8F8F2"
    property string iconColor: "#F8F8F2"

    width: 280
    height: 720
    // color: root.backGrColor
    border.width: 4
    radius: 8
    border.color: root.boderColor
    Column {
        Rectangle {
            id: titleFolderDialog
            width: root.width
            height: 40
            border.width: 4
            border.color: root.boderColor
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
                source: "qrc:/svg/folder"
                radius: 5
                onClicked: folderDialog.open()
            }
            TextLong {
                x: 35
                anchors.verticalCenter: parent.verticalCenter
                id: openFolderTextItem
                width: parent.width-45
                height: parent.height-10
                text: playerService.folderPath
                pixelSize: 20
                bold: true
                color: root.textColor
                running: true
                delay: 1000
            }
        }
        Rectangle {
            width: root.width;
            height: root.height-titleFolderDialog.height
            border.width: 4
            border.color: root.boderColor
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
                    clip: true


                    MouseArea {
                        anchors.fill: itemDelegate
                        onClicked: {
                            playerService.mediaIndex = index
                        }
                    }
                    Row {
                        Text {
                            id: indexElement
                            width: 35
                            height: 20
                            font.pixelSize: 18
                            font.bold: itemDelegate.ListView.isCurrentItem
                            text: (index+1) + ": "
                            color: itemDelegate.ListView.isCurrentItem?root.text2Color:root.textColor
                        }
                        Column {
                            TextLong {
                                // anchors.verticalCenter: parent.verticalCenter
                                id: textElement
                                y: 0
                                width: itemDelegate.width-45
                                height: 25
                                text: modelData["title"].toString()
                                pixelSize: 16
                                bold: itemDelegate.ListView.isCurrentItem
                                color: itemDelegate.ListView.isCurrentItem?root.text2Color:root.textColor
                                running: itemDelegate.ListView.isCurrentItem
                                delay: 1000
                            }
                            Text {
                                y: 20
                                width: itemDelegate.width-45
                                height: 20
                                font.pixelSize: 14
                                font.bold: itemDelegate.ListView.isCurrentItem
                                text: "artist: " + modelData["artist"].toString()
                                color: itemDelegate.ListView.isCurrentItem?root.text2Color:root.textColor
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
