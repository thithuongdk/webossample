
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import app.playerservice 1.0

Rectangle {
    id: mediaPath
    width: 280
    height: 720
    border.width: 2
    Item {
        x: 30
        y: 5
        width: 250
        height: 20
        Column {
            Text {
                id: pathInfo
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 10
                color: "black"
                text: "/media/multimedia"
            }
        }
    }
    Button {
        x: 5
        y: 5
        width: 20
        height: 20
        id: openFolderButton
        text: "..."
        onClicked: folderDialog.open()
    }
    FolderDialog {
        id: folderDialog
        currentFolder: "file:///media/multimedia"
        onAccepted: {
            playerservice.folderPath = folderDialog.selectedFolder.replace("file://", playerservice.storagePath)
            pathInfo.text = folderDialog.selectedFolder.replace("file://", "")
            folderDialog.currentFolder = folderDialog.selectedFolder
        }
        // /onRejected: {
            // playerservice.folderPath = "storage:///media/multimedia"
            // pathInfo.text = playerservice.folderPath
        // }
    }
}