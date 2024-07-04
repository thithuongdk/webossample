
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
        Column {
            spacing: 5
            Text {
                id: currentStoragePath
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 10
                color: "black"
                text: playerService.storagePath
            }
            Text {
                id: currentFolderPath
                y: 20
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 10
                color: "black"
                text: playerService.folderPath
            }
            Text {
                id: currentFolderDialog
                y: 40
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 10
                color: "black"
                text: folderDialog.currentFolder.toString()
            }
            
        }
        // ListModel {
        //     id: listMusicModel
        //     ListElement {
        //         name: "storagePathInfo"
        //         svalue: playerService.storagePath
        //     }
        //     ListElement {
        //         name: "storagePathInfo"
        //         svalue: playerService.storagePath
        //     }
        // }
        // ListView {
        //     width: 180; height: 200
        //     model: listMusicModel
        //     delegate: 
        //         Text {
        //             color: "black"
        //             text: name + ": " + svalue
        //         }
        // }
        // ListView {
        //     y: 100
        //     model: playerservice.mediaList.getArray()
        //     delegate: Column {
        //         Text {
        //             font.pointSize: 10
        //             text: modelData.getObject().get("title").asString()
        //         }
        //         Text {
        //             font.pointSize: 8
        //             text: modelData.getObject().get("artist").asString()
        //         }
        //     }
        // }
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
        // /onRejected: {
            // playerservice.folderPath = "storage:///media/multimedia"
            // pathInfo.text = playerservice.folderPath
        // }
    }
    
    // ListView {
    //     id: musicList
    //     anchors.fill: parent
    //     model: musicModel
    //     delegate: musicDelegate

    //     property var musicModel: []

    //     Component {
    //         id: musicDelegate
    //         Rectangle {
    //             width: musicListView.width
    //             height: 50
    //             color: ListView.isCurrentItem ? "#e0e0e0" : "transparent"

    //             Text {
    //                 anchors.verticalCenter: parent.verticalCenter
    //                 anchors.left: parent.left
    //                 anchors.leftMargin: 16
    //                 text: fileName
    //                 font.pixelSize: 16
    //             }

    //             MouseArea {
    //                 anchors.fill: parent
    //                 onClicked: {
    //                     musicListView.currentIndex = index
    //                     musicPlayer.source = filePath
    //                     musicPlayer.play()
    //                 }
    //             }
    //         }
    //     }

    //     Component.onCompleted: {
    //         // Lấy danh sách tệp nhạc từ thư mục âm nhạc
    //         var musicDir = Qt.resolvedUrl("file:///path/to/music/directory/")
    //         var dirModel = pbnjson::JValue      mediaList
    //         dirModel.folder = musicDir
    //         for (var i = 0; i < dirModel.count; i++) {
    //             var fileInfo = dirModel.get(i)
    //             if (fileInfo.fileName.toLowerCase().endsWith(".mp3") || fileInfo.fileName.toLowerCase().endsWith(".wav")) {
    //                 musicModel.push({
    //                     fileName: fileInfo.fileName,
    //                     filePath: musicDir + fileInfo.fileName
    //                 })
    //             }
    //         }
    //     }
        
    //     loadListMusic(json) {

    //     }
    // }
}