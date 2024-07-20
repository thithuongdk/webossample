
import QtQuick
import QtQuick.Controls.Basic

Slider {
    id: root
    property real iconSize: 30
    property string textColor: "black"
    property string text2Color: "brown"
    property string leftText: ""
    property string rightText: ""
    property string leftIcon: "qrc:/png/help"
    property string rightIcon: "qrc:/png/help"

    snapMode: Slider.SnapOnRelease
    stepSize: 0.1

    background: Item{
        id: backGround
        Item{
            anchors.left: backGround.left
            width: childrenRect.width
            height: root.iconSize
            Text {
                id: leftIconText
                visible: root.leftText !== ""
                height: root.iconSize
                font.family: "Helvetica"
                font.pointSize: root.iconSize
                verticalAlignment: Text.AlignTop
                color: root.textColor
                text: root.leftText
            }
            Image {
                id: leftIconImage
                visible: root.leftIcon !== "qrc:/png/help"
                source: root.leftIcon
                width: root.iconSize
                height: root.iconSize
                y: root.iconSize*0.3
                // anchors.verticalCenter: leftIconText.verticalCenter
                fillMode: Image.PreserveAspectFit
                // sourceSize.width: 1024
                // sourceSize.height: 1024
                // color: "black"
                opacity: 1
                // scale: 0.01
                smooth: true
                mipmap: true
            }
        }
        Item{
            anchors.right: backGround.right
            width: childrenRect.width
            height: root.iconSize
            Text {
                id: rightIconText
                visible: root.rightText !== ""
                height: root.iconSize
                font.family: "Helvetica"
                font.pointSize: root.iconSize
                verticalAlignment: Text.AlignTop
                color: root.textColor
                text: rightText
            }
            Image {
                id: rightIconImage
                visible: root.rightIcon !== "qrc:/png/help"
                source: root.rightIcon
                width: root.iconSize
                height: root.iconSize
                // anchors.verticalCenter: leftIconText.verticalCenter
                y: root.iconSize*0.2
                fillMode: Image.PreserveAspectFit
                // sourceSize.width: 1024
                // sourceSize.height: 1024
                // color: "black"
                opacity: 1
                // scale: 0.01
                smooth: true
                mipmap: true
            }
        }
        Rectangle {
            id: sliderLine
            y: root.height - 30
            height: 4
            width: root.width
            radius: 2
            color: root.text2Color

            Rectangle {
                x: 0
                width: root.visualPosition * parent.width
                height: parent.height
                color: root.textColor
                radius: 2
            }
        }
    }

    handle: Rectangle {
        id: sliderBall
        x: root.visualPosition * (root.width-width)
        y: sliderLine.y + (sliderLine.height-height)/2
        width: 20
        height: 20
        radius: 10
        color: root.pressed ? "#505050" : root.textColor
        border.color: root.textColor
    }
}
