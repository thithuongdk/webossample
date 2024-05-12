
import QtQuick 2.15

Rectangle {
    id: button
    width: 80
    height: 80
    color: "gray"
    property bool pressed: false
    property string source: ""
    signal clicked

    Action {
        id: ownAction
        iconSource: !button.action || __iconOverriden ? button.iconSource : ""
        iconName: !button.action || __iconOverriden ? button.iconName : ""
    }
    Connections {
        target: action
        onTriggered: button.clicked()
    }

    function onPressedButtonChanged() {}

    Image {
        id: image
        source: source
        width: 50
        height: 50
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        opacity: pressed ? 0.5 : 0.9
        scale: pressed ? 1.2 : 1
    }
    MouseArea {
        id: behavior
        anchors.fill: image
        onClicked: {
            onClickedButton();
        }
        onPressedChanged: {
            onPressedButtonChanged();
        }
    }
}