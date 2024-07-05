
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import app.playerservice 1.0
// import libpbnjson 1.0


Rectangle {
    id: mediaPath
    width: 280
    height: 720
    border.width: 2
    Item {
        x: 30
        y: 5
        width: 250
        height: 200
        ListModel {
            id: pathInfoModel
            ListElement { name: "storage Path";     svalue: "" }
            ListElement { name: "folder Path";      svalue: "" }
            ListElement { name: "current Folder";   svalue: "" }
        }
        Component.onCompleted: {
            playerService.onStoragePathChanged.connect(function(newValue) {
                pathInfoModel.setProperty(0, "svalue", playerService.storagePath);});
            playerService.onFolderPathChanged.connect(function(newValue) {
                pathInfoModel.setProperty(0, "svalue", playerService.folderPath);});
            folderDialog.onCurrentFolderChanged.connect(function(newValue) {
                pathInfoModel.setProperty(0, "svalue", folderDialog.currentFolder.toString());});
        }
        ListView {
            width: 180; height: 200
            model: pathInfoModel
            delegate: 
                Text {
                    color: "black"
                    text: model.name + ": " + model.svalue
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
        currentFolder: "file:///"
        onAccepted: {
            playerService.folderPath = folderDialog.selectedFolder.toString().replace("file://", playerService.storagePath)
            currentFolderPath.text = playerService.folderPath
            currentStoragePath.text = playerService.storagePath
        }
    }
}