import QtQuick 2.10
import QtQuick.Controls 2.2

Item {
    Pane {
        id: visualizationRoot
        anchors.fill: parent
    }
    Component.onCompleted: {
        console.log("Start visualization")
        editor.visualize(visualizationRoot)
    }
}
