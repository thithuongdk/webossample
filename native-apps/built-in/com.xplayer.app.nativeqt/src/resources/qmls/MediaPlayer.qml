
import QtQuick
import QtQuick.Controls
import app.playerservice 1.0


Rectangle {
    id: mediaPlayer
    x: 0
    width: 800
    height: 720
    border.width: 2
    
    Item {
        x: 100
        y: 100
        width: 200
        height: 200
        Column {
            spacing: 5
            Text {
                id: musicNameInfo
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.musicPath.substring(playerService.musicPath.lastIndexOf("/") + 1)
            }
            Text {
                id: musicPathInfo
                y: 20
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.musicPath
            }
            Text {
                id: mediaIndexInfo
                y: 40
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.mediaIndex.toFixed(0).toString() + "/" + (playerService.mediaCount-1).toFixed(0).toString()
            }
            Text {
                id: mediaIdInfo
                y: 60
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.mediaId
            }
            Text {
                id: playStateInfo
                y: 80
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: (playerService.playState==0)?"stop":((playerService.playState==1)?"pause":"play")
            }
            Text {
                id: seekInfo
                y: 100
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.seek.toFixed(0).toString()
            }
            Text {
                id: folderPathInfo
                y: 120
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.folderPath
            }
            Text {
                id: storagePathInfo
                y: 140
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 18
                color: "black"
                text: playerService.storagePath
            }
        }
    }

    Rectangle {
        x: 0
        y: 500
        width: parent.width
        height: 200
        border.width: 2
        color: "lightgray"

        IconSlider {
            id: durationSlider
            x: 40
            y: 20
            width: parent.width-x*2
            height: 80
            from: 0
            value: playerService.seek
            to: (playerService.duration*1000).toFixed(0)
            iconSize: 30
            leftIsText: true
            leftText: fmtime((value/1000).toFixed(0))
            rightIsText: true
            rightText: fmtime((playerService.duration).toFixed(0))
            onValueChanged: playerService.seek=(value.toFixed(0));
            function fmtime(ss) {
                var h = Math.floor(ss/3600).toFixed(0);
                var m = Math.floor((ss%3600)/60).toFixed(0);
                var s = Math.floor(ss%60).toFixed(0);
                return (h==0)?(m+":"+s):(h+":"+m+":"+s);
            }
        }

        IconSlider {
            id: rateSlider
            x: 40
            y: 100
            width: 150
            height: 80
            from: 0
            value: playerService.rate
            to: 4.0
            iconSize: 30
            leftIsText: false
            leftSrc: "qrc:/png/time"
            rightIsText: true
            rightText: value.toFixed(1).toString()
            onValueChanged: playerService.rate=(value.toFixed(1));
        }

        IconSlider {
            id: volumeSlider
            x: durationSlider.x + durationSlider.width - width
            y: 100
            width: 150
            height: 80
            from: 0
            value: playerService.volume
            to: 100
            iconSize: 30
            leftIsText: true
            leftText: playerService.volume.toString()
            rightIsText: false
            rightSrc: value.toFixed(0)==0? "qrc:/png/speaker_x"
                                :"qrc:/png/speaker_"
                                + (value*3/100).toFixed(0).toString()
            onValueChanged: playerService.volume=(value.toFixed(0));
        }

        Item {
            id: controlButton
            property real iconSize: 65
            // x: 0
            y: 100
            // width: 800
            anchors.horizontalCenter: parent.horizontalCenter
            height: 100
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                IconButton {
                    id: rewindButton
                    pointSize: controlButton.iconSize
                    source: "qrc:/png/rewind"
                    onClicked: {
                        playerService.seek=(Math.max(0,playerService.seek - 10000))
                    }
                }
                IconButton {
                    id: backButton
                    pointSize: controlButton.iconSize
                    source: "qrc:/png/back"
                    onClicked: {
                        playerService.mediaIndex=(playerService.mediaIndex-1)
                    }
                }
                IconButton {
                    id: playButton
                    pointSize: controlButton.iconSize
                    source: (playerService.playState==2)?"qrc:/png/pause":"qrc:/png/play"
                    onClicked: {
                        if(playerService.playState==2) {
                            playerService.playState=(1);
                        } else {
                            playerService.playState=(2);
                        }
                        
                    }
                }
                IconButton {
                    id: stopButton
                    pointSize: controlButton.iconSize
                    source: "qrc:/png/stop"
                    onClicked: {
                        playerService.playState=(0);
                    }
                }
                IconButton {
                    id: nextButton
                    pointSize: controlButton.iconSize
                    source: "qrc:/png/next"
                    onClicked: {
                        playerService.mediaIndex=(playerService.mediaIndex+1)
                    }
                }
                IconButton {
                    id: skipButton
                    pointSize: controlButton.iconSize
                    source: "qrc:/png/skip"
                    onClicked: {
                        playerService.seek=(Math.min(playerService.duration*1000,playerService.seek + 10000))
                    }
                }
            }
        }
    }
}