import QtQuick 2.0
import QtQuick.Shapes 1.0

Row {
    id: root
    property alias socketView: parameterSocket
    property bool isInput: true
    property var socket
    property var nodeView

    layoutDirection: isInput ? Qt.LeftToRight : Qt.RightToLeft
    anchors.right: isInput ? undefined : parent.right
    spacing: 2

    signal socketClicked(variant parameter)
    signal socketEntered(variant socket)
    signal socketExited(variant socket)

    function getSocketCenter() {
        return socketMouseArea.mapToGlobal(socketMouseArea.width / 2, socketMouseArea.height / 2)
    }

    Socket {
        id: parameterSocket
        MouseArea {
            id: socketMouseArea
            anchors.fill: parameterSocket
            anchors.margins: -4
            hoverEnabled: isInput
            onEntered: socketEntered(parameterSocket)
            onExited: socketExited(parameterSocket)
            onClicked: {
                socketClicked(root)
            }
        }
        color: socket && socket.color || "grey"
    }

    Text {
        id: parameterName
        text: socket && socket.label
    }
}

