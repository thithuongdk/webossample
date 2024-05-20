
import QtQuick
import QtQuick.Window

Window {
    visible: true
    MediaPath {
        id: mediapath
        width: 280
        height: 720
        border.width: 2
    }
    MediaPlayer {
        id: mediaplayer
        x: mediapath.width
        width: 800
        height: 720
        border.width: 2
    }
}