import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import scivi 0.1
import ".."

Page {
    property bool isNew: false
    property string path: ""

    ZoomPanView {
        id: zoomPanView
        clip: true
        anchors.fill: parent

        onClicked: {
            graphView.spaceClicked()
        }

        GraphView {
            id: graphView
            parent: zoomPanView.innerView
            anchors.fill: parent
            transformOrigin: Item.TopLeft
        }
    }

    footer: ToolBar {
        Row {
            ToolButton {
                icon.source: "add_black.png"
                onClicked: {
                    stackView.push("PalettePage.qml")
                }
            }
            ToolButton {
                icon.source: "delete_black.png"
                onClicked: {
                    if (editor.selectedNode) {
                        editor.deleteNodeModel(editor.selectedNode)
                    }
                }
            }
            ToolButton {
                icon.source: "settings_black.png"
                onClicked: {
                    if (editor.selectedNode) {
                        stackView.push("NodeSettingsPage.qml");
                    }
                }
            }
            ToolSeparator {}
            ToolButton {
                icon.source: "play_black.png"
                onClicked: {
                    stackView.push("../VisualizationPage.qml")
                }
            }
            ToolButton {
                id: saveButton
                icon.source: "save_black.png"
                onClicked: {
                    editor.exportDiagram("dataflow.json");
                }
            }
        }
    }

    Component.onCompleted: {
        editor.setGraphView(graphView)
        if (isNew) {
            editor.setupFromFile(path)
        } else {
            editor.importDiagramFrom(path)
        }
    }
}
