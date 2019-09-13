import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import scivi 0.1

NodeView {
    id: root
    property alias text: label.text
    signal positionChanged()

    onXChanged: positionChanged()
    onYChanged: positionChanged()

    Rectangle {
        id: outline
        border.color: "blue"
        border.width: 1
        anchors.fill: background
        anchors.margins: -2
        radius: 8
        color: "transparent"
        visible: isSelected
    }

    Rectangle {
        id: background
        anchors.fill: body
        anchors.topMargin: -4
        anchors.bottomMargin: -8

        border.color: "black"
        color: '#d6d6d6'
        radius: 8

        Drag.active: nodeMouseArea.drag.active

        MouseArea {
            id: nodeMouseArea
            anchors.fill: parent
            drag.target: root
            onClicked: {
                bodyClicked(root, mouse.modifiers)
            }

            onPositionChanged: {
                root.node.nodeX = root.x
                root.node.nodeY = root.y
            }
        }
    }

    Column {
        id: body
        width: Math.max(label.implicitWidth + 8, Math.max(outputsColumn.implicitWidth, inputsColumn.implicitWidth))
        spacing: 4

        Text {
            id: label
            font.bold: true
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            text: node.name
        }
        Column {
            id: outputsColumn
            anchors.right: body.right
            anchors.rightMargin: -6
            spacing: 4
        }
        Column {
            id: inputsColumn
            anchors.left: body.left
            anchors.leftMargin: -6
            spacing: 4
        }
        Row {
            anchors.right: body.right
            anchors.rightMargin: 2
            spacing: 4

            RoundButton {
                width: 14
                height: 14
                text: "-"
                font.pointSize: 8
                visible: root.node.conceptId === -1
            }

            RoundButton {
                width: 14
                height: 14
                text: "X"
                font.pointSize: 8
                hoverEnabled: false
                onClicked: {
                    root.deleteClicked(root)
                }
            }
        }
    }

    function onParameterClicked(parameter) {
        if (parameter.isInput) {
            root.inputClicked(root, parameter.socketView, parameter.socket)
        } else {
            root.outputClicked(root, parameter.socketView, parameter.socket)
        }
    }

    function createParameter(column, args) {
        var parameterComponent = Qt.createComponent("Parameter.qml")
        var parameterObject = parameterComponent.createObject(column, args)
        parameterObject.socketClicked.connect(onParameterClicked)
        if (parameterObject.isInput) {
            parameterObject.socketEntered.connect(root.inputEntered)
            parameterObject.socketExited.connect(root.inputExited)
            root.inputCreated(parameterObject.socketView)
        } else {
            parameterObject.socketEntered.connect(root.outputEntered)
            parameterObject.socketExited.connect(root.outputExited)
            root.outputCreated(parameterObject.socketView)
        }
    }

    function onInputsChanged() {
        var parameterComponent = Qt.createComponent("Parameter.qml")
        var inputs = node.inputs
        if (inputs) {
            for (var i = 0; i < inputs.length; ++i) {
                var inputArgs = {
                    socket: inputs[i],
                    nodeView: root
                }
                createParameter(inputsColumn, inputArgs)
            }
        }
    }

    function onOutputsChanged() {
        var parameterComponent = Qt.createComponent("Parameter.qml")
        var outputs = node.outputs
        if (outputs) {
            for (var i = 0; i < outputs.length; ++i) {
                var outputArgs = {
                    isInput: false,
                    socket: outputs[i],
                    nodeView: root
                }
                createParameter(outputsColumn, outputArgs)
            }
        }
    }

    Component.onCompleted: {
        var parameterComponent = Qt.createComponent("Parameter.qml")
        node.inputsChanged.connect(onInputsChanged)
        node.outputsChanged.connect(onOutputsChanged)
        if (node.inputs) {
            onInputsChanged()
        }
        if (node.outputs) {
            onOutputsChanged()
        }
    }
}
