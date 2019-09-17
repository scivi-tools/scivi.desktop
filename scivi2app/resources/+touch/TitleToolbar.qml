import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property alias title: titleLabel.text
    height: toolbar.height

    ToolBar {
        id: toolbar
        anchors.fill: parent
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                }
            }
        }

        Label {
            id: titleLabel
            anchors.centerIn: parent
        }
    }
}
