
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

    width: 280
    height: 720
    border.width: 2
    radius: 6
    border.color: mediaPath.boderColor
    color: mediaPath.backGrColor
    FolderDialog {
        id: folderDialog
        currentFolder: "file://" + playerService.folderPath
        onAccepted: {
            playerService.folderPath = folderDialog.selectedFolder.toString().replace("file://", "")
        }
    }
    Column {
        anchors.fill: parent
        spacing: 2
        Row {
            id: titleFolderDialog
            spacing: 2
            anchors.top: parent.top
            anchors.topMargin: 10
            width: parent.width;
            height: 25
            IconButton {
                id: openFolderButton
                anchors.left: parent.left
                anchors.leftMargin: 5
                pointSize: 20
                source: "qrc:/svg/menu"
                iconColor: mediaPlayer.textColor
                onClicked: folderDialog.open()
            }
            Text {
                anchors.left: openFolderButton.right
                anchors.right: parent.right
                anchors.rightMargin: 10
                text: playerService.folderPath
                font.pixelSize: 20
                font.bold: true
                color: mediaPath.textColor
            }
        }
        ListView {
            anchors.top: titleFolderDialog.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            id: musicFileListView
            model: playerService.mediaList
            currentIndex: playerService.mediaIndex
            flickDeceleration: 3000
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.SnapToItem
            clip: true
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
        }
    }
    Component.onCompleted: {
        playerService.onMediaListChanged.connect(function(newValue) {
            musicFileListView.currentIndex = Qt.binding(function() { return playerService.mediaIndex; });
        });
    }
}