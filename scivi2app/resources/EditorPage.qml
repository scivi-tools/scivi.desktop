import QtQuick 2.10
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5 as Controls2

import scivi 0.1

Item {
    SplitView {
        id: root
        anchors.fill: parent
        orientation: Qt.Horizontal

        ColumnLayout {
            id: leftColumn
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            spacing: 0

            Palette {
                id: entityList
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 128
            }

            RowLayout {

                Controls2.Button {
                    id: collapseAllButton
                    text: qsTr("Collapse all")
                    Layout.fillWidth: true
                    onClicked: {
                        entityList.collapseAll();
                    }
                }

                Controls2.Button {
                    id: expandAllButton
                    text: qsTr("Expand all")
                    Layout.fillWidth: true
                    onClicked: {
                        entityList.expandAll();
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            anchors.bottom: parent.bottom
            spacing: 0

            Controls2.TabBar {
                id: tabBar
                width: parent.width
                Layout.fillHeight: false
                Layout.fillWidth: false
                visible: count > 1

                ClosableTabButton {
                    width: 100
                    text: qsTr("Diagram")
                    onCloseClicked: {
                        console.log(tabBar.children.length);
                    }
                }
            }

            StackLayout {
                id: contentStack
                width: parent.width
                currentIndex: tabBar.currentIndex

                Item {
                    id: diagramBar

                    SplitView {
                        anchors.fill: parent

                        ZoomPanView {
                            id: zoomPanView
                            clip: true
                            Layout.fillWidth: true

                            onClicked: {
                                graphView.spaceClicked()
                            }

                            MouseArea {
                                id: connectMouseArea
                                z: -1
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                propagateComposedEvents: true
                                hoverEnabled: true

                                onPositionChanged: {
                                    if (!editor.isConnecting) {
                                        mouse.accepted = false;
                                    } else {
                                        var x = mouse.x
                                        var y = mouse.y
                                        var localPoint = Qt.point(connectMouseArea.mouseX, connectMouseArea.mouseY)
                                        var globalPoint = connectMouseArea.mapToGlobal(x, y)
                                        graphView.mousePositionChanged(globalPoint.x, globalPoint.y)
                                    }
                                }
                            }

                            GraphView {
                                id: graphView
                                parent: zoomPanView.innerView
                                anchors.fill: parent
                                transformOrigin: Item.TopLeft
                            }
                        }

                        NodeInspector {
                            id: nodeInspector
                            width: parent.width * 0.10
                            Layout.fillHeight: true
                            node: editor.selectedNode
                            visible: editor.selectedNode !== null
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            entityList.itemClicked.connect(editor.paletteItemClicked)
            editor.setGraphView(graphView)
        }
    }

    Component {
        id: tabButtonComponent
        ClosableTabButton {
            width: 100
        }
    }

    function addTab(view, text) {
        view.parent = contentStack;
        var index = tabBar.count;
        var tabItem = tabButtonComponent.createObject(view, { text: text });
        var handler = function () {
            view.parent = null;
            tabBar.removeItem(tabItem);
        }
        tabItem.closeClicked.connect(handler);
        tabBar.addItem(tabItem);
        tabBar.currentIndex = tabBar.count - 1
    }
}
