
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    Rectangle {
        id: mediapath
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
    Rectangle {
        id: mediaplayer
        x: mediapath.width
        width: 800
        height: 720
        border.width: 2

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
                PropertyChanges { target: playbutton; source: "qrc:/png/stop" }
            },
            State {
                name: "play"
                PropertyChanges { target: playbutton; source: "qrc:/png/play" }
            },
            State {
                name: "pause"
                PropertyChanges { target: playbutton; source: "qrc:/png/pause" }
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
                    text: mediaplayer.state
                }
                Text {
                    id: durationinfo
                    y: 100
                    width: parent.width
                    font.family: "Helvetica"
                    font.pointSize: 50
                    color: "black"
                    text: mediaplayer.current_duration.toFixed(0).toString()
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

            Slider {
                id: durationSlider
                x: 40
                y: 0
                width: parent.width-x*2
                height: 100
                from: 0
                value: 50
                to: mediaplayer.max_duration
                snapMode: Slider.SnapOnRelease
                stepSize: 0.1

                background: Item{
                    Text {
                        id: maxdurationText
                        y: 5
                        width: 100
                        font.family: "Helvetica"
                        font.pointSize: 20
                        color: "black"
                        text: mediaplayer.current_duration.toFixed(0).toString()
                    }
                    Text {
                        id: durationText
                        x: durationSlider.width - width
                        y: 5
                        width: 70
                        font.family: "Helvetica"
                        font.pointSize: 20
                        color: "black"
                        text: mediaplayer.max_duration.toFixed(0).toString()
                    }
                    Rectangle {
                        x: durationSlider.leftPadding
                        y: durationSlider.topPadding + durationSlider.availableHeight / 2 - height / 2
                        implicitHeight: 4
                        width: durationSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "brown"

                        Rectangle {
                            width: durationSlider.visualPosition * parent.width
                            height: parent.height
                            color: "black"
                            radius: 2
                        }
                    }
                }

                handle: Rectangle {
                    x: durationSlider.leftPadding + durationSlider.visualPosition * (durationSlider.availableWidth - width)
                    y: durationSlider.topPadding + durationSlider.availableHeight / 2 - height / 2
                    implicitWidth: 20
                    implicitHeight: 20
                    radius: 10
                    color: durationSlider.pressed ? "#505050" : "#0f0f0f"
                    border.color: "#bdbebf"
                }

                onValueChanged: {
                    mediaplayer.current_duration = value;
                }
            }

            Slider {
                id: speedSlider
                x: durationSlider.x
                y: 100
                width: 150
                height: 100
                from: 0
                value: 1
                to: mediaplayer.max_speed
                snapMode: Slider.SnapOnRelease
                stepSize: 0.1

                background: Item{
                    Image {
                        id: speedImage
                        source: "qrc:/png/time"
                        x: 0
                        y: 0
                        width: 30
                        height: 30
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        id: speedText
                        x: speedImage.width+10
                        y: 0
                        width: 50
                        font.family: "Helvetica"
                        font.pointSize: 20
                        color: "black"
                        text: mediaplayer.current_speed.toFixed(1).toString()
                    }
                    Rectangle {
                        x: speedSlider.leftPadding
                        y: speedSlider.topPadding + speedSlider.availableHeight / 2 - height / 2
                        implicitHeight: 4
                        width: speedSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "brown"

                        Rectangle {
                            width: speedSlider.visualPosition * parent.width
                            height: parent.height
                            color: "black"
                            radius: 2
                        }
                    }
                }

                handle: Rectangle {
                    x: speedSlider.leftPadding + speedSlider.visualPosition * (speedSlider.availableWidth - width)
                    y: speedSlider.topPadding + speedSlider.availableHeight / 2 - height / 2
                    implicitWidth: 20
                    implicitHeight: 20
                    radius: 10
                    color: speedSlider.pressed ? "#505050" : "#0f0f0f"
                    border.color: "#bdbebf"
                }

                onValueChanged: {
                    mediaplayer.current_speed = value;
                }
            }

            Slider {
                id: volumeSlider
                x: durationSlider.x + durationSlider.width - width
                y: 100
                width: 150
                height: 100
                from: 0
                value: 50
                to: mediaplayer.max_volume
                snapMode: Slider.SnapOnRelease
                stepSize: 0.1

                background: Item{
                    Image {
                        id: volumeImage
                        source: mediaplayer.current_volume==0? "qrc:/png/speaker_x"
                                    :"qrc:/png/speaker_"
                                    + (mediaplayer.current_volume*3/mediaplayer.max_volume).toFixed(0).toString()
                        x: volumeSlider.availableWidth - width
                        y: 0
                        width: 30
                        height: 30
                        fillMode: Image.PreserveAspectFit
                    }
                    Text {
                        id: volumeText
                        x: volumeImage.x - width-10
                        y: 0
                        width: 50
                        font.family: "Helvetica"
                        font.pointSize: 20
                        color: "black"
                        text: mediaplayer.current_volume.toFixed(0).toString()
                    }
                    Rectangle {
                        x: volumeSlider.leftPadding
                        y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                        implicitHeight: 4
                        width: volumeSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: "brown"

                        Rectangle {
                            width: volumeSlider.visualPosition * parent.width
                            height: parent.height
                            color: "black"
                            radius: 2
                        }
                    }
                }
                

                handle: Rectangle {
                    x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                    y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                    implicitWidth: 20
                    implicitHeight: 20
                    radius: 10
                    color: volumeSlider.pressed ? "#505050" : "#0f0f0f"
                    border.color: "#bdbebf"
                }

                onValueChanged: {
                    mediaplayer.current_volume = value;
                }
            }

            Item {
                // x: 0
                y: 100
                // width: 800
                anchors.horizontalCenter: parent.horizontalCenter
                height: 100
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item {
                        width: 80
                        height: 80
                        Image {
                            property bool pressed: false
                            id: rewindbutton
                            source: "qrc:/png/rewind"
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
                            source: "qrc:/png/back"
                            width: 50
                            height: 50
                            smooth: true
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
                            source: "qrc:/png/stop"
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
                            source: "qrc:/png/next"
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
                            source: "qrc:/png/skip"
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
                    // IconButton {
                    //     width: 80
                    //     height: 80
                    //     // source: "qrc:/png/skip"
                    //     // clickAction: {
                    //     //     mediaplayer.state = stop;
                    //     // }
                    // }
                }
            }
        }
    }
}