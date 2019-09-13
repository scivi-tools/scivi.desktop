import QtQuick 2.10
import QtQml 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0 as Platforms
import scivi 0.1
import QtCharts 2.2
import QtDataVisualization 1.0

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("SciVi Editor")

    style: ApplicationWindowStyle {
        background: Rectangle {
            color: 'white'
        }
    }

    Item {
        id: pluginContainer
        anchors.centerIn: parent
        width: firmwareGenDialogLoader.width
        height: firmwareGenDialogLoader.height
        Loader {
            id: firmwareGenDialogLoader
            width: 600
            height: 400
            active: false
        }
    }

    Platforms.FileDialog {
        id: openDialog
        fileMode: Platforms.FileDialog.OpenFile
        title: "Please choose a ontology file"
        nameFilters: ["Ontolis files (*.ont)"]

        onAccepted: {
            editor.setupFromFile(openDialog.file);
        }
    }

    Platforms.FileDialog {
        id: saveDialog
        fileMode: Platforms.FileDialog.SaveFile
        defaultSuffix: ".json"
        title: "Please choose file to export"
        nameFilters: ["JSON file (*.json)"]
        onAccepted: {
            editor.exportDiagram(saveDialog.file)
        }
    }

    Platforms.FileDialog {
        id: importDialog
        fileMode: Platforms.FileDialog.OpenFile
        title: "Please choose a diagram file"
        nameFilters: ["JSON file (*.json)"]
        onAccepted: {
            editor.importDiagramFrom(importDialog.file, {diagram: editor.diagram})
        }
    }

    Action {
        id: exit
        text: qsTr("&Quit")
        shortcut: StandardKey.Quit
        onTriggered: Qt.quit()
    }

    Action {
        id: generateFirmware
        text: qsTr("Firmware generator")
        onTriggered: {
            firmwareGenDialogLoader.active = true
            firmwareGenDialogLoader.item.setDiagram(editor.diagram)
            firmwareGenDialogLoader.item.open()
        }
    }

    Action {
        shortcut: StandardKey.Cancel
        onTriggered: {
            if (stack.state === "visualize") {
                stack.pop();
            }
        }
    }

    Action {
        id: combineNodes
        text: qsTr("Combine nodes")
        onTriggered: {
            popup.open();
        }
    }

    Dialog {
        id: popup
        modal: true
        anchors.centerIn: Overlay.overlay
        title: qsTr("Enter name of new node")
        standardButtons: Dialog.Ok | Dialog.Cancel
        contentItem: TextInput {
            id: nodeNameInput
            text: qsTr("NewNode")
        }
        onAccepted: {
            if (nodeNameInput.text.length > 0) {
                editor.combineSelectedNodes(nodeNameInput.text)
            }
        }
    }

    menuBar: MenuBar {
        id: menuBar
        Menu {
            id: fileMenu
            title: qsTr("File")

            MenuItem {
                text: qsTr("New")
                shortcut: StandardKey.New
                onTriggered: openDialog.open()
            }
            MenuItem {
                text: qsTr("Open")
                shortcut: StandardKey.Open
                onTriggered: importDialog.open()
            }

            MenuItem {
                text: qsTr("Save diagram as")
                shortcut: StandardKey.SaveAs
                onTriggered: saveDialog.open()
            }
            MenuItem {
                text: qsTr("Save diagram")
                shortcut: StandardKey.Save
                onTriggered: {
                    if (editor.diagramPath !== "") {
                        editor.exportDiagram(editor.diagramPath);
                    } else {
                        saveDialog.open();
                    }
                }
            }

            MenuSeparator {}
            MenuItem { action: exit }
        }

        Menu {
            id: toolsMenu
            title: qsTr("Tools")
            Menu {
                id: pluginsMenu
                title: qsTr("Plugins")
                Instantiator {
                    model: pluginManager.pluginModel
                    MenuItem {
                        text: model.display
                        onTriggered: {
                            var view = pluginManager.runPluginWithName(model.display);
                            stack.currentItem.addTab(view, model.display)
                        }
                    }
                    onObjectAdded: pluginsMenu.insertItem(index, object)
                    onObjectRemoved: pluginsMenu.removeItem(object)
                }
            }

            MenuSeparator {

            }

            MenuItem {
                id: visualizeMenu
                text: qsTr("Visualize")
                shortcut: "Ctrl+R"
                onTriggered: {
                    stack.state = "visualize"
                }
            }
            MenuItem { action: combineNodes }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: dataflowEditorComponent

        states: [
            State {
                PropertyChanges {
                    target: visualizeMenu
                    text: qsTr("Visualize")
                    onTriggered: {
                        stack.state = "visualize"
                    }
                }
            },
            State {
                name: "visualize"
                PropertyChanges {
                    target: visualizeMenu
                    text: qsTr("Stop visualization")
                    onTriggered: {
                        stack.state = ""
                        stack.pop(visualizationComponent)
                    }
                }
                StateChangeScript {
                    script: {
                        stack.push(visualizationComponent)
                    }
                }
            }
        ]
    }

    Component {
        id: dataflowEditorComponent
        EditorPage {
        }
    }

    Component {
        id: visualizationComponent
        VisualizationPage {
        }
    }

    statusBar: StatusBar {
        RowLayout {
            Label {
                id: statusBarLabel
                text: logger.msg
                color: logger.msgType === 'error' || logger.msgType === 'warning' ? 'red' : 'black'
            }
        }
    }

    Component.onCompleted: {
        firmwareGenDialogLoader.active = true;
    }
}
