
import QtQuick
import QtQuick.Controls
import app.playerservice 1.0


Rectangle {
    id: root

    property string boderColor: "#ABE338"
    property string backGrColor: "#0F1B07"
    property string textColor: "#FFD700"
    property string text2Color: "#F8F8F2"
    property string iconColor: "#F8F8F2"

    width: 1000
    height: 720
    border.color: root.boderColor
    // color: root.backGrColor
    Column {
        Rectangle {
            width: root.width
            height: 520
            border.width: 4
            radius: 8
            border.color: root.boderColor

            Column {
                x: 100
                y: 100
                Text {
                    text: playerService.musicPath.toString().substring(playerService.musicPath.toString().lastIndexOf("/") + 1)
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaData["title"].toString()
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaData["artist"].toString()
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaData["album"].toString()
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: (playerService.mediaIndex<0?"-":(playerService.mediaIndex+1).toFixed(0).toString()) + "/" + playerService.mediaCount.toFixed(0).toString();
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaId
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaPipeId
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: (playerService.playState===0)?"stop":((playerService.playState===1)?"pause":"play")
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.seek
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.musicPath.toString()
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.mediaData["uri"].toString()
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.folderPath
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: playerService.storagePath
                    font.pixelSize: 18
                    font.bold: true
                    color: root.textColor
                }
                Text {
                    text: "⏮️⯇⏯️▶▸🞂⏹️⯈⏭️🔁🔂🔀▶️⏸️"
                    font.pixelSize: 40
                    font.bold: true
                    font.family: "Geneva"
                    color: root.textColor
                }
            }
        }

        Rectangle {
            width: root.width
            height: 200
            border.width: 4
            radius: 8
            border.color: root.boderColor
            // color: root.backGrColor

            IconSlider {
                id: durationSlider
                x: 40
                y: 20
                width: parent.width-x*2
                height: 80
                from: 0
                value: playerService.seek
                to: (playerService.duration*1000).toFixed(0)
                iconSize: 25
                textColor: root.textColor
                text2Color: root.text2Color
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
                y: 110
                width: 150
                height: 70
                from: 0
                value: playerService.rate
                to: 40
                iconSize: 20
                textColor: root.textColor
                text2Color: root.text2Color
                leftIcon: "qrc:/svg/time"
                rightText: (value/10).toFixed(1).toString()
                onValueChanged: playerService.rate=value.toFixed(0);
            }

            IconSlider {
                id: volumeSlider
                x: durationSlider.x + durationSlider.width - width
                y: 110
                width: 150
                height: 70
                from: 0
                value: playerService.volume
                to: 100
                iconSize: 20
                textColor: root.textColor
                text2Color: root.text2Color
                leftText: playerService.volume.toString()
                rightIcon: value<1? "qrc:/svg/volume_x":
                            (value<33?"qrc:/svg/volume_0":
                            (value<66?"qrc:/svg/volume_1":"qrc:/svg/volume_2"))
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
                    Item {
                        y: controlButton.iconSize/2
                        height: controlButton.iconSize/2
                        width: controlButton.iconSize/2
                        Text {
                            anchors.centerIn: parent
                            text: playerService.shuffleStatus===2?"1":""
                            font.bold: true
                            font.pixelSize: controlButton.iconSize/8
                        }
                        IconButton {
                            id: shuffleButton
                            radius: controlButton.iconSize/10
                            pointSize: controlButton.iconSize/2
                            source: playerService.shuffleStatus===0?"qrc:/svg/repeat":
                                    (playerService.shuffleStatus===1?"qrc:/svg/repeat":
                                    (playerService.shuffleStatus===2?"qrc:/svg/repeat":"qrc:/svg/shuffle"))
                            enableBg: false
                            opacity: playerService.shuffleStatus===0?0.5:1
                            iconColor: playerService.shuffleStatus===0? "#60000000":root.text2Color
                            bgColor: "transparent"
                            onClicked: {
                                playerService.shuffleStatus=playerService.shuffleStatus+1
                            }
                        }
                    }
                    IconButton {
                        id: rewindButton
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize*0.7
                        source: "qrc:/svg/fast_back"
                        iconColor: root.iconColor
                        bgColor:root.textColor
                        onClicked: {
                            playerService.seek=(Math.max(0,playerService.seek - 10000))
                        }
                    }
                    IconButton {
                        id: backButton
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize*0.7
                        source: "qrc:/svg/back"
                        iconColor: root.iconColor
                        bgColor:root.textColor
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
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize
                        source: playerService.playState===0?"qrc:/svg/play_pause":
                                (playerService.playState===1?"qrc:/svg/play_circle":"qrc:/svg/pause_circle")
                        iconColor: root.iconColor
                        bgColor:root.textColor
                        onClicked: {
                            if(playerService.playState===2) {
                                playerService.playState=1;
                            } else {
                                playerService.playState=2;
                            }
                            
                        }
                    }
                    IconButton {
                        id: stopButton
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize
                        source: "qrc:/svg/stop"
                        iconColor: root.iconColor
                        bgColor:root.textColor
                        onClicked: {
                            playerService.playState=(0);
                        }
                    }
                    IconButton {
                        id: nextButton
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize*0.7
                        source: "qrc:/svg/forward"
                        iconColor: root.iconColor
                        bgColor:root.textColor
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
                        anchors.verticalCenter: parent.verticalCenter
                        pointSize: controlButton.iconSize*0.7
                        source: "qrc:/svg/fast_forward"
                        iconColor: root.iconColor
                        bgColor:root.textColor
                        onClicked: {
                            playerService.seek=(Math.min(playerService.duration*1000,playerService.seek + 10000))
                        }
                    }
                }
            }
        }
    }
}
