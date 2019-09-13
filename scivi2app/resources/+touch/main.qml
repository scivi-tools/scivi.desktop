import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("SciVi")

    header: ToolBar {
        visible: stackView.currentItem.title || false

        Item {
            anchors.fill: parent

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
                text: stackView.currentItem.title || ""
                anchors.centerIn: parent
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.qml"
        anchors.fill: parent
    }

    onClosing: {
        if (stackView.depth > 1) {
            close.accepted = false
            stackView.pop()
        } else {
            return;
        }
    }
}
