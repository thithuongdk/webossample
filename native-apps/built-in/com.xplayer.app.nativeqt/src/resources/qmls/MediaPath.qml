
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Rectangle {
    id: mediaPath
    width: 280
    height: 720
    border.width: 2
    property string music_path: "/media/internal/downloads/"
    Item {
        x: 0
        y: 100
        width: 280
        height: 200
        Column {
            Text {
                id: pathinfo
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
        text: "Open Folder Picker"
        onClicked: {
            // pathinfo.text = mediaplayer.current_duration.toFixed(0).toString()
            folderDialog.open()
            // music_path = "/media/internal/downloads/"
        }
    }
    FolderDialog {
        id: folderDialog
        currentFolder: "/media/internal/downloads/"
        // selectedFolder: viewer.folder
    }
    // MyViewer {
    //     id: viewer
    //     folder: folderDialog.selectedFolder
    // }
}