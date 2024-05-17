
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs


Rectangle {
    id: mediaPlayer
    x: 0
    width: 800
    height: 720
    border.width: 2

    property real max_duration: 1000
    property alias current_duration: durationSlider.value
    property real max_speed: 4.0
    property alias current_speed: speedSlider.value
    property real max_volume: 100
    property alias current_volume: volumeSlider.value

    state: "stop"
    states: [
        State {
            name: "stop"
            PropertyChanges { target: playButton; source: "qrc:/png/stop" }
        },
        State {
            name: "play"
            PropertyChanges { target: playButton; source: "qrc:/png/play" }
        },
        State {
            name: "pause"
            PropertyChanges { target: playButton; source: "qrc:/png/pause" }
        }
    ]
    Item {
        x: 100
        y: 100
        width: 200
        height: 200
        Column {
            Text {
                id: playinginfo
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 50
                color: "black"
                text: mediaPlayer.state
            }
            Text {
                id: durationinfo
                y: 100
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 50
                color: "black"
                text: mediaPlayer.current_duration.toFixed(0).toString()
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
            value: 0
            to: mediaPlayer.max_duration
            iconSize: 30
            leftIsText: true
            leftText: mediaPlayer.current_duration.toFixed(0).toString()
            rightIsText: true
            rightText: mediaPlayer.max_duration.toFixed(0).toString()
        }

        IconSlider {
            id: speedSlider
            x: 40
            y: 100
            width: 150
            height: 80
            from: 0
            value: 1
            to: mediaPlayer.max_speed
            iconSize: 30
            leftIsText: false
            leftSrc: "qrc:/png/time"
            rightIsText: true
            rightText: mediaPlayer.current_speed.toFixed(1).toString()
        }

        IconSlider {
            id: volumeSlider
            x: durationSlider.x + durationSlider.width - width
            y: 100
            width: 150
            height: 80
            from: 0
            value: 50
            to: mediaPlayer.max_volume
            iconSize: 30
            leftIsText: true
            leftText: mediaPlayer.current_volume.toFixed(0).toString()
            rightIsText: false
            rightSrc: mediaPlayer.current_volume==0? "qrc:/png/speaker_x"
                                :"qrc:/png/speaker_"
                                + (mediaPlayer.current_volume*3/mediaPlayer.max_volume).toFixed(0).toString()
        }

        Item {
            // x: 0
            y: 100
            // width: 800
            anchors.horizontalCenter: parent.horizontalCenter
            height: 100
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                IconButton {
                    id: rewindButton
                    pointSize: 80
                    source: "qrc:/png/rewind"
                    onClicked: {
                        mediaPlayer.current_duration = Math.max(0,mediaPlayer.current_duration - 10);
                    }
                }
                IconButton {
                    id: backButton
                    pointSize: 80
                    source: "qrc:/png/back"
                    onClicked: {
                        mediaPlayer.current_duration = 0;
                    }
                }
                IconButton {
                    id: playButton
                    pointSize: 80
                    source: "qrc:/png/stop"
                    onClicked: {
                        mediaPlayer.state = (mediaPlayer.state==="play") ? "pause":"play";
                    }
                }
                IconButton {
                    id: nextButton
                    pointSize: 80
                    source: "qrc:/png/next"
                    onClicked: {
                        mediaPlayer.current_duration = mediaPlayer.max_duration;
                    }
                }
                IconButton {
                    id: skipButton
                    pointSize: 80
                    source: "qrc:/png/skip"
                    onClicked: {
                        mediaPlayer.current_duration = Math.min(mediaPlayer.max_duration,mediaPlayer.current_duration + 10);
                    }
                }
            }
        }
    }
}