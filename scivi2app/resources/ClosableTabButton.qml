import QtQuick 2.0
import QtQuick.Controls 2.3

TabButton {
    signal closeClicked()

    Text {
        id: closeText
        text: "x"
        width: 16
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    MouseArea {
        anchors.fill: closeText
        onClicked: closeClicked()
    }
}
