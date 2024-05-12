
import QtQuick 2.15

Item {
    id: mediaplayer
    width: 1080
    height: 720

    property real max_duration: 1000
    property real current_duration: 200
    property real max_speed: 4
    property real current_speed: 1
    property real max_volume: 100
    property real current_volume: 50

    state: "stop"
    states: [
        State {
            name: "stop"
            PropertyChanges { target: playbutton; source: "qrc:/src/resources/icons/png/stop.png" }
        },
        State {
            name: "play"
            PropertyChanges { target: playbutton; source: "qrc:/src/resources/icons/png/play.png" }
        },
        State {
            name: "pause"
            PropertyChanges { target: playbutton; source: "qrc:/src/resources/icons/png/pause.png" }
        }
    ]
    Item {
        x: 100
        y: 100
        width: 200
        height: 200
        Column {
            Text {
                id: playingtext
                y: 0
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 50
                color: "black"
                text: mediaplayer.state
            }
            Text {
                id: durationtext
                y: 100
                width: parent.width
                font.family: "Helvetica"
                font.pointSize: 50
                color: "black"
                text: current_duration.toFixed(0).toString()
            }
        }
    }
    Rectangle {
        x: 0
        y: 500
        width: 1080
        height: 200
        border.width: 2
        color: "lightgray"
        Item {
            id: durationSlider
            x: 0
            y: 0
            width: 1080
            height: 100
            Rectangle {
                id: durationContainer
                x: 40
                y: 45
                width: 1000
                height: 4
                radius: 2
                color: "brown"
                Rectangle {
                    id: durationLine
                    x: 0
                    y: 0
                    width: durationConsole.x+durationConsole.width/2
                    height: 4
                    radius: 2
                    color: "black"
                }
                Rectangle {
                    id: durationConsole
                    x: durationContainer.width * mediaplayer.current_duration / mediaplayer.max_duration - width/2
                    anchors.verticalCenter: parent.verticalCenter
                    width: 10
                    height: 10
                    radius: 5
                    color: "black"
                }
            }
            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                    if (pressed) {
                        mediaplayer.current_duration = Math.max(0 , Math.min(durationContainer.width , mouseX-durationContainer.x))
                                        * max_duration / durationContainer.width    
                    }
                }
            }
        }
        Item {
            id: speedSlider
            x: 40
            y: 120
            width: 150
            height: 100
            Image {
                id: speedImage
                source: "qrc:/src/resources/icons/png/time.png"
                x: 0
                y: 0
                width: 30
                height: 30
                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: speedText
                anchors.left: speedImage.right
                y: 0
                width: 50
                font.family: "Helvetica"
                font.pointSize: 20
                color: "black"
                text: current_speed.toFixed(1).toString()
            }
            Rectangle {
                id: speedContainer
                x: 0
                y: 40
                width: 150
                height: 4
                radius: 2
                color: "brown"
                Rectangle {
                    id: speedLine
                    x: 0
                    y: 0
                    width: speedConsole.x+speedConsole.width/2
                    height: 4
                    radius: 2
                    color: "black"
                }
                Rectangle {
                    id: speedConsole
                    x: speedContainer.width * mediaplayer.current_speed / mediaplayer.max_speed - width/2
                    anchors.verticalCenter: parent.verticalCenter
                    width: 10
                    height: 10
                    radius: 5
                    color: "black"
                }
            }
            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                    if (pressed) {
                        mediaplayer.current_speed = Math.max(0 , Math.min(speedContainer.width , mouseX-speedContainer.x))
                                        * max_speed / speedContainer.width    
                    }
                }
            }
        }
        Item {
            id: volumeSlider
            x: 890
            y: 120
            width: 150
            height: 100
            Image {
                id: volumeImage
                source: current_volume==0? "qrc:/src/resources/icons/png/speaker_x.png"
                            :"qrc:/src/resources/icons/png/speaker_" + (current_volume*3/max_volume).toFixed(0).toString() + ".png"
                anchors.right: parent.right
                y: 0
                width: 30
                height: 30
                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: volumeText
                anchors.right: volumeImage.left
                y: 0
                width: 50
                font.family: "Helvetica"
                font.pointSize: 20
                color: "black"
                text: current_volume.toFixed(0).toString()
            }
            Rectangle {
                id: volumeContainer
                anchors.right: parent.right
                y: 40
                width: 150
                height: 4
                radius: 2
                color: "brown"
                Rectangle {
                    id: volumeLine
                    x: 0
                    y: 0
                    width: volumeConsole.x+volumeConsole.width/2
                    height: 4
                    radius: 2
                    color: "black"
                }
                Rectangle {
                    id: volumeConsole
                    x: volumeContainer.width * mediaplayer.current_volume / mediaplayer.max_volume - width/2
                    anchors.verticalCenter: parent.verticalCenter
                    width: 10
                    height: 10
                    radius: 5
                    color: "black"
                }
            }
            MouseArea {
                anchors.fill: parent
                onPositionChanged: {
                    if (pressed) {
                        mediaplayer.current_volume = Math.max(0 , Math.min(volumeContainer.width , mouseX-volumeContainer.x))
                                        * max_volume / volumeContainer.width    
                    }
                }
            }
        }
        Item {
            x: 0
            y: 100
            width: 1080
            height: 100
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                Item {
                    width: 80
                    height: 80
                    Image {
                        property bool pressed: false
                        id: rewindbutton
                        source: "qrc:/src/resources/icons/png/rewind.png"
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        opacity: pressed ? 0.5 : 0.9
                        scale: pressed ? 1.2 : 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                            }
                            onPressedChanged: {
                                parent.pressed = pressed;
                            }
                        }
                    }
                }
                Item {
                    width: 80
                    height: 80
                    Image {
                        property bool pressed: false
                        id: backbutton
                        source: "qrc:/src/resources/icons/png/back.png"
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        opacity: pressed ? 0.5 : 0.9
                        scale: pressed ? 1.2 : 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                            }
                            onPressedChanged: {
                                parent.pressed = pressed;
                            }
                        }
                    }
                }
                Item {
                    width: 80
                    height: 80
                    Image {
                        property bool pressed: false
                        id: playbutton
                        source: "qrc:/src/resources/icons/png/stop.png"
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        opacity: pressed ? 0.5 : 0.9
                        scale: pressed ? 1.2 : 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                mediaplayer.state = (mediaplayer.state==="play") ? "pause":"play";
                            }
                            onPressedChanged: {
                                parent.pressed = pressed;
                            }
                        }
                    }
                }
                Item {
                    width: 80
                    height: 80
                    Image {
                        property bool pressed: false
                        id: nextbutton
                        source: "qrc:/src/resources/icons/png/next.png"
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        opacity: pressed ? 0.5 : 0.9
                        scale: pressed ? 1.2 : 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                            }
                            onPressedChanged: {
                                parent.pressed = pressed;
                            }
                        }
                    }
                }
                Item {
                    width: 80
                    height: 80
                    Image {
                        property bool pressed: false
                        id: skipbutton
                        source: "qrc:/src/resources/icons/png/skip.png"
                        width: 50
                        height: 50
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        opacity: pressed ? 0.5 : 0.9
                        scale: pressed ? 1.2 : 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                            }
                            onPressedChanged: {
                                parent.pressed = pressed;
                            }
                        }
                    }
                }
            }
        }
    }
}