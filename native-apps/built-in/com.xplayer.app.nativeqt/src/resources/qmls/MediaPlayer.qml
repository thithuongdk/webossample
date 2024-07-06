
import QtQuick
import QtQuick.Controls
import app.playerservice 1.0


Rectangle {
    id: mediaPlayer

    property var boderColor: "#ABE338"
    property var backGrColor: "#0F1B07"
    property var textColor: "#FFD700"
    property var text2Color: "#F8F8F2"

    x: 0
    width: 1000
    height: 720
    border.width: 2
    radius: 4
    border.color: mediaPlayer.boderColor
    color: mediaPlayer.backGrColor
    Item {
        x: 100
        y: 100
        width: 200
        height: 200

        Column {
            Text {
                text: playerService.musicPath.toString().substring(playerService.musicPath.toString().lastIndexOf("/") + 1)
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaData["title"].toString()
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaData["artist"].toString()
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaData["album"].toString()
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: (playerService.mediaIndex<0?"-":(playerService.mediaIndex+1).toFixed(0).toString()) + "/" + playerService.mediaCount.toFixed(0).toString();
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaId
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaPipeId
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: (playerService.playState==0)?"stop":((playerService.playState==1)?"pause":"play")
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.seek
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.musicPath.toString()
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.mediaData["uri"].toString()
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.folderPath
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: playerService.storagePath
                font.pixelSize: 18
                font.bold: true
                color: mediaPlayer.textColor
            }
            Text {
                text: "⏮️⯇⏯️▶▸🞂⏹️⯈⏭️"
                font.pixelSize: 40
                font.bold: true
                color: mediaPlayer.textColor
            }
        }
    }


    Rectangle {
        x: 0
        y: 520
        width: parent.width
        height: 200
        border.width: 2
        radius: 8
        border.color: mediaPlayer.boderColor
        color: mediaPlayer.backGrColor

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
            textColor: mediaPlayer.textColor
            text2Color: mediaPlayer.text2Color
            leftText: fmtime((value/1000).toFixed(0))
            rightText: fmtime((playerService.duration).toFixed(0))
            onValueChanged: playerService.seek=(value.toFixed(0));
            function fmtime(ss) {
                var h = Math.floor(ss/3600).toFixed(0);
                var m = Math.floor((ss%3600)/60).toFixed(0);
                var s = Math.floor(ss%60).toFixed(0);
                var timeString = "";
                if (h > 0) {
                    timeString += h + ":" + (m<10 ? "0" : "");
                }
                timeString += m + ":" + (s<10 ? "0":"") + s;
                return timeString;
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
            textColor: mediaPlayer.textColor
            text2Color: mediaPlayer.text2Color
            leftText: "⏲"
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
            textColor: mediaPlayer.textColor
            text2Color: mediaPlayer.text2Color
            leftText: playerService.volume.toString()
            rightText: value.toFixed(0)==0? "🔇":(value<33?"🔈": (value<66?"🔉": "🔊"))
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
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                IconButton {
                    id: rewindButton
                    pointSize: controlButton.iconSize
                    source: "⏮️"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
                    onClicked: {
                        playerService.seek=(Math.max(0,playerService.seek - 10000))
                    }
                }
                IconButton {
                    id: backButton
                    pointSize: controlButton.iconSize
                    source: "⯇"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
                    onClicked: {
                        playerService.mediaIndex=(playerService.mediaIndex-1)
                    }
                }
                IconButton {
                    id: playButton
                    pointSize: controlButton.iconSize
                    source: "⏯️"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
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
                    source: "⏹️"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
                    onClicked: {
                        playerService.playState=(0);
                    }
                }
                IconButton {
                    id: nextButton
                    pointSize: controlButton.iconSize
                    source: "⯈"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
                    onClicked: {
                        playerService.mediaIndex=(playerService.mediaIndex+1)
                    }
                }
                IconButton {
                    id: skipButton
                    pointSize: controlButton.iconSize
                    source: "⏭️"
                    iconColor: mediaPlayer.backGrColor
                    bgColor:mediaPlayer.textColor
                    onClicked: {
                        playerService.seek=(Math.min(playerService.duration*1000,playerService.seek + 10000))
                    }
                }
            }
        }
    }
}