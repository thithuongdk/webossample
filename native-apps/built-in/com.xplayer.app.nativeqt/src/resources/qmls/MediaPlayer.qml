
import QtQuick
import QtQuick.Controls
import app.playerservice 1.0


Rectangle {
    id: mediaPlayer

    property string boderColor: "#ABE338"
    property string backGrColor: "#0F1B07"
    property string textColor: "#FFD700"
    property string text2Color: "#F8F8F2"
    property string iconColor: "#F8F8F2"

    width: 1000
    height: 720
    border.color: mediaPlayer.boderColor
    // color: mediaPlayer.backGrColor
    Column {
        Rectangle {
            width: mediaPlayer.width
            height: 520
            border.width: 4
            radius: 8
            border.color: mediaPlayer.boderColor

            Column {
                x: 100
                y: 100
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
                    text: "⏮️⯇⏯️▶▸🞂⏹️⯈⏭️🔁🔂🔀▶️⏸️"
                    font.pixelSize: 40
                    font.bold: true
                    font.family: "Geneva"
                    color: mediaPlayer.textColor
                }
            }
        }

        Rectangle {
            width: mediaPlayer.width
            height: 200
            border.width: 4
            radius: 8
            border.color: mediaPlayer.boderColor
            // color: mediaPlayer.backGrColor

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
                to: 40
                iconSize: 30
                textColor: mediaPlayer.textColor
                text2Color: mediaPlayer.text2Color
                leftText: "⏲"
                rightText: (value/10).toFixed(1).toString()
                onValueChanged: playerService.rate=value.toFixed(0);
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
                rightText: value<1? "🔇":(value<33?"🔈": (value<66?"🔉": "🔊"))
                onValueChanged: playerService.volume=value.toFixed(0);
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
                        id: shuffleButton
                        y: controlButton.iconSize/2
                        pointSize: controlButton.iconSize/2
                        source: playerService.shuffleStatus==0?"🔁":(playerService.shuffleStatus==1?"🔁":(playerService.shuffleStatus==2?"🔂":"🔀"))
                        enableBg: false
                        iconColor: playerService.shuffleStatus==0? "#60000000":mediaPlayer.text2Color
                        bgColor: "transparent"
                        onClicked: {
                            playerService.shuffleStatus=playerService.shuffleStatus+1
                        }
                    }
                    IconButton {
                        id: rewindButton
                        pointSize: controlButton.iconSize
                        source: "⏮️"
                        iconColor: mediaPlayer.iconColor
                        bgColor:mediaPlayer.textColor
                        onClicked: {
                            playerService.seek=(Math.max(0,playerService.seek - 10000))
                        }
                    }
                    IconButton {
                        id: backButton
                        pointSize: controlButton.iconSize
                        source: "⏪"
                        iconColor: mediaPlayer.iconColor
                        bgColor:mediaPlayer.textColor
                        onClicked: {
                            if(playerService.mediaCount>1) {
                                playerService.mediaIndex=(playerService.mediaIndex-1)
                            } else {
                                playerService.seek=0;
                            }
                        }
                    }
                    IconButton {
                        id: playButton
                        pointSize: controlButton.iconSize
                        source: playerService.playState==0?"⏯️":(playerService.playState==1?"⯈":"⏸️")
                        iconColor: mediaPlayer.iconColor
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
                        iconColor: mediaPlayer.iconColor
                        bgColor:mediaPlayer.textColor
                        onClicked: {
                            playerService.playState=(0);
                        }
                    }
                    IconButton {
                        id: nextButton
                        pointSize: controlButton.iconSize
                        source: "⏩"
                        iconColor: mediaPlayer.iconColor
                        bgColor:mediaPlayer.textColor
                        onClicked: {
                            if(playerService.mediaCount>1) {
                                playerService.mediaIndex=(playerService.mediaIndex+1)
                            } else {
                                playerService.seek=0;
                            }
                        }
                    }
                    IconButton {
                        id: skipButton
                        pointSize: controlButton.iconSize
                        source: "⏭️"
                        iconColor: mediaPlayer.iconColor
                        bgColor:mediaPlayer.textColor
                        onClicked: {
                            playerService.seek=(Math.min(playerService.duration*1000,playerService.seek + 10000))
                        }
                    }
                }
            }
        }
    }
}
