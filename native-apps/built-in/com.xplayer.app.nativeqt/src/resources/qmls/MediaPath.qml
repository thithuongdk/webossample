
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: mediaPath
    width: 280
    height: 720
    border.width: 2
    property string music_path: "/media/internal/"
    Item {
        x: 0
        y: 100
        width: 280
        height: 200
        Column {
            Text {
                id: pathInfo
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 10
                color: "black"
                text: music_path
            }
        }
    }
    Button {
        id: openFolderButton
        text: "Open Folder Picker"
        onClicked: folderDialog.open()
    }
    FolderDialog {
        id: folderDialog
        currentFolder: "/media/internal/"
        selectedFolder: "/media/internal/"
        onAccepted: {
            music_path = folderDialog.selectedFolder
            pathInfo.text = music_path
        }
        onRejected: {
            music_path = "cdcasdm"
            pathInfo.text = music_path
        }
    }
}