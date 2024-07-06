
import QtQuick
import QtQuick.Controls

Slider {
    id: container
    property real iconSize: 30
    property var textColor: "black"
    property var text2Color: "brown"
    property alias leftIsText: leftIconText.visible
    property alias leftText: leftIconText.text
    property alias leftSrc: leftIconImage.source
    property alias rightIsText: rightIconText.visible
    property alias rightText: rightIconText.text
    property alias rightSrc: rightIconImage.source

    snapMode: Slider.SnapOnRelease
    stepSize: 0.1

    background: Item{
        Item{
            Text {
                id: leftIconText
                visible: false
                height: container.iconSize
                font.family: "Helvetica"
                font.pointSize: container.iconSize*2/3
                verticalAlignment: Text.AlignTop
                color: container.textColor
                text: ""
            }
            Image {
                id: leftIconImage
                visible: !leftIconText.visible
                width: container.iconSize
                height: container.iconSize
                fillMode: Image.PreserveAspectFit
                source: ""
            }
        }
        Item{
            x: container.width-childrenRect.width
            Text {
                id: rightIconText
                visible: false
                height: container.iconSize
                font.family: "Helvetica"
                font.pointSize: container.iconSize*2/3
                verticalAlignment: Text.AlignTop
                color: container.textColor
                text: ""
            }
            Image {
                id: rightIconImage
                visible: !rightIconText.visible
                width: container.iconSize
                height: container.iconSize
                fillMode: Image.PreserveAspectFit
                source: ""
            }
        }
        Rectangle {
            id: sliderLine
            y: container.height - 30
            height: 4
            width: container.width
            radius: 2
            color: container.text2Color

            Rectangle {
                x: 0
                width: container.visualPosition * parent.width
                height: parent.height
                color: container.textColor
                radius: 2
            }
        }
    }

    handle: Rectangle {
        id: sliderBall
        x: container.visualPosition * (container.width-width)
        y: sliderLine.y + (sliderLine.height-height)/2
        width: 20
        height: 20
        radius: 10
        color: container.pressed ? "#505050" : container.textColor
        border.color: container.textColor
    }
}
