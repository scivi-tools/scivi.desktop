import QtQuick 2.0
import QtQuick.Controls 2.4
import ".."

Page {
    title: qsTr("Node settings")
    NodeInspector {
        id: nodeInspector
        anchors.fill: parent
        node: editor.selectedNode
        visible: editor.selectedNode !== null
    }
}
