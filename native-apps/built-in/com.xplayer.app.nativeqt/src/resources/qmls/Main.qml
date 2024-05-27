
import QtQuick
import QtQuick.Window

Window {
    id: mediaWindow
    visible: true
    width: 1080
    height: 720
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