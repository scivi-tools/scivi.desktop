import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

/* global stackView */
Page {
    id: page
    property bool isNew: false
    property string nameFilter: "*.*"

    Component {
        id: fileSelectionComponent

        FileSelectionPage {
            id: fileSelectionPage
            nameFilters: page.nameFilter
            onFileSelected: {
                var filePath = currentFolder() + "/" + fileName;
                console.log("user select file" + currentFolder() + "/" + fileName)
                stackView.replace("EditorPage.qml", { isNew: page.isNew, path: filePath })
            }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent

        Image {
            source: "images/scivi_logo_website.png"
            Layout.alignment: Qt.AlignCenter
        }
        Button {
            text: qsTr("Create new diagram")
            Layout.preferredWidth: 200
            onClicked: {
                page.isNew = true
                page.nameFilter = "*.ont"
                stackView.push(fileSelectionComponent)
            }
        }
        Button {
            text: qsTr("Open diagram")
            Layout.preferredWidth: 200
            onClicked: {
                page.isNew = false
                page.nameFilter = "*.json"
                stackView.push(fileSelectionComponent)
            }
        }
    }

    Component.onCompleted: {
        // TODO: Open view depends on previous file
    }
}
