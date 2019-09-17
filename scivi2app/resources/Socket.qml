import QtQuick 2.0

Item {
    property alias color: socketCircle.color

    width: socketCircle.width
    height: socketCircle.height

    Rectangle {
        id: socketCircle
        anchors.centerIn: parent
        width: 12
        height: 12
        radius: width / 2
        border.color: "black"
    }
}
