
import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    height: 45
    width: 200
    property bool bold: false
    property bool running: true
    property string text: ""
    property string color: "black"
    property real delay: 2000
    property real pixelSize: 20
    clip: true
    Text {
        id: textElement
        y:0
        x:0
        height: parent.height
        width: parent.width
        font.pixelSize: root.pixelSize
        font.bold: root.bold
        text: root.text
        color: root.color
        SequentialAnimation on x {
            running: root.running && (root.width < textElement.implicitWidth)
            PauseAnimation {
                duration: root.delay
            }
            PropertyAnimation {
                from: textElement.x
                to: -textElement.width
                loops: Animation.Infinite
                duration: 20*textElement.width
            }
            onStopped: {
                textElement.x = 0
            }
        }
    }
}
