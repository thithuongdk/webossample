
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import app.playerservice 1.0
// import libpbnjson 1.0



Rectangle {
    id: mediaPath

    property var boderColor: "#5C821A"
    property var backGrColor: "#0F1B07"
    property var textColor: "#C6D166"

    width: 280
    height: 720
    border.width: 2
    radius: 8
    border.color: mediaPath.boderColor
    color: mediaPath.backGrColor
    Item {
        x: 5
        y: 5
        width: 250
        height: 200

        Button {
            width: 20
            height: 20
            id: openFolderButton
            text: "..."
            onClicked: folderDialog.open()
        }
        FolderDialog {
            id: folderDialog
            currentFolder: "file://" + playerService.folderPath
            onAccepted: {
                playerService.folderPath = folderDialog.selectedFolder.toString().replace("file://", "")
            }
        }
        Text {
            x: 30
            text: playerService.folderPath + musicFileListView.currentIndex
            font.pixelSize: 18
            font.bold: true
            color: mediaPath.textColor
        }
    }
    ListView {
        x: 5
        y: 30
        width: 270; height: 200
        id: musicFileListView
        model: playerService.mediaList
        currentIndex: playerService.mediaIndex
        delegate: Item {
            id: itemDelegate
            width: parent.width
            height: 30
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    playerService.mediaIndex = index
                }
            }
            SequentialAnimation {
                id: textAnimation
                running: musicFileListView.currentIndex == index && musicFileListView.width < textElement.implicitWidth
                
                PauseAnimation {
                    duration: 5000
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
                    font.pixelSize: 14
                    font.bold: itemDelegate.ListView.isCurrentItem
                    text: (index+1) + ": " + modelData["title"].toString()
                    color: mediaPath.textColor
                }
                Text {
                    font.pixelSize: 10
                    font.bold: itemDelegate.ListView.isCurrentItem
                    text: "artist: " + modelData["artist"].toString()
                    color: mediaPath.textColor
                }
            }
        }
        flickDeceleration: 3000
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapToItem
        clip: true
    }
    Component.onCompleted: {
        playerService.onMediaListChanged.connect(function(newValue) {
            musicFileListView.currentIndex = Qt.binding(function() { return playerService.mediaIndex; });
        });
    }
}