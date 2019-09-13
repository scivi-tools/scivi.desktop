import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    property var node
//    clip: true

    Page {
        id: propertyColumn
        anchors.fill: parent

        header: Row {
            Text {
                text: "Selected node: "
            }
            Text {
                text: node ? node.name : "null"
            }
        }

        ListView {
            id: settingsListView
            model: node ? node.settingsModel : null
            anchors.fill: parent

            delegate: Component {
                Column {
                    width: settingsListView.width
                    Text {
                        text: name
                    }
                    Loader {
                        property var setter: function(name, value) {
                            settingsListView.model.saveValue(name, value);
                        };
                        width: settingsListView.width
                        sourceComponent: fieldComponent
                    }
                }
            }
        }
    }

    Text {
        id: errorMessage
        anchors.centerIn: parent
        text: "Node not selected"
    }

    states: [
        State {
            name: "nodeSelected"
            when: node !== null && node !== undefined
            PropertyChanges {
                target: errorMessage
                visible: false
            }
            PropertyChanges {
                target: propertyColumn
                visible: true
            }
        },
        State {
            name: "nodeNotSelected"
            when: node === null
            PropertyChanges {
                target: errorMessage
                visible: true
            }
            PropertyChanges {
                target: propertyColumn
                visible: false
            }
        }
    ]
}
