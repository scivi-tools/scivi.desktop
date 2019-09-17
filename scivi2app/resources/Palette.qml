import QtQuick 2.0
import QtQuick.Controls 1.4

TreeView {
    id: objectsList

    signal itemClicked(variant object, int depth)

    model: editor.paletteModel

    itemDelegate: Rectangle {
        height: 30
        width: parent.width

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.value
            font.pointSize: 10
        }
        MouseArea {
            id: itemMouseArea
            anchors.fill: parent
            onClicked: {
                if (objectsList.isExpanded(styleData.index)) {
                    objectsList.collapse(styleData.index)
                } else {
                    objectsList.expand(styleData.index)
                }
            }
        }
        Connections {
            target: itemMouseArea
            onClicked: {
                if (styleData.depth > 0) {
                    itemClicked(styleData.index, styleData.depth)
                }
            }
        }
    }

    TableViewColumn {
        title: qsTr("Nodes")
        role: "name_role"
    }

    function expandAll() {
        for(var i = 0; i < model.rowCount(); i++) {
            var index = model.index(i, 0);
            if(!objectsList.isExpanded(index)) {
                objectsList.expand(index);
            }
        }
    }

    function collapseAll() {
        for (var i = 0; i < model.rowCount(); i++) {
            var index = model.index(i, 0);
            if (objectsList.isExpanded(index)) {
                objectsList.collapse(index);
            }
        }
    }
}
