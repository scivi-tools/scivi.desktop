import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    id: root
    property alias nameFilters: dialog.nameFilters
    signal fileSelected(string fileName)

    function currentFolder() {
        return dialog.currentFolder()
    }

    header: TitleToolbar {
        title: qsTr("Select file")
    }

    FilePicker {
        showDotAndDotDot: true
        id: dialog
        anchors.fill: parent
        onFileSelected: {
            root.fileSelected(fileName)
        }
    }
}
