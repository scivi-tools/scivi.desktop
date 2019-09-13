import QtQuick 2.0
import QtQuick.Controls 2.4
import scivi 0.1
import "../"

Page {
    title: "Select item"

    Palette {
        id: entityList
        anchors.fill: parent
        onItemClicked: {
            stackView.pop()
        }
    }

    Component.onCompleted: {
         entityList.itemClicked.connect(editor.paletteItemClicked)
    }
}
