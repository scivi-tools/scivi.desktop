#include "node.h"

#include <QJsonObject>
#include <memory>

#include "./nodeview.h"

namespace scivi {
namespace diagram {

int Node::lastGeneratedId = 0;

Node::Node(QObject *parent) : QObject(parent) {
    m_id = Node::lastGeneratedId++;
}

Node::Node(QList<SharedNodeSocket> inputs, QList<SharedNodeSocket> outputs,
           QObject *parent)
    : QObject(parent), m_inputs(inputs), m_outputs(outputs) {
    m_id = Node::lastGeneratedId++;
}

Node::~Node() {
    if (m_view != nullptr) m_view->deleteLater();
    m_inputs.clear();
    m_outputs.clear();
}

int Node::id() const { return m_id; }

float Node::x() const { return m_x; }

float Node::y() const { return m_y; }

int Node::inputCount() const { return m_inputs.size(); }

NodeSocket *Node::inputAt(int index) const { return m_inputs.at(index).data(); }

int Node::outputCount() const { return m_outputs.size(); }

NodeSocket *Node::outputAt(int index) const {
    return m_outputs.at(index).data();
}

int Node::inputCount(QQmlListProperty<NodeSocket> *property) {
    return reinterpret_cast<Node *>(property->data)->inputCount();
}

NodeSocket *Node::inputAt(QQmlListProperty<NodeSocket> *property, int index) {
    return reinterpret_cast<Node *>(property->data)->inputAt(index);
}

int Node::outputCount(QQmlListProperty<NodeSocket> *property) {
    return reinterpret_cast<Node *>(property->data)->outputCount();
}

NodeSocket *Node::outputAt(QQmlListProperty<NodeSocket> *property, int index) {
    return reinterpret_cast<Node *>(property->data)->outputAt(index);
}

QQmlListProperty<NodeSocket> Node::inputsProp() {
    return QQmlListProperty<NodeSocket>(this, this, &Node::inputCount,
                                        &Node::inputAt);
}

QQmlListProperty<NodeSocket> Node::outputsProp() {
    return QQmlListProperty<NodeSocket>(this, this, &Node::outputCount,
                                        &Node::outputAt);
}

QList<SharedNodeSocket> Node::inputs() const { return m_inputs; }

QList<SharedNodeSocket> Node::outputs() const { return m_outputs; }

NodeView *Node::view() { return m_view; }

void Node::setX(float x) { m_x = x; }

void Node::setY(float y) { m_y = y; }

void Node::setInputs(QList<SharedNodeSocket> inputs) {
    m_inputs = inputs;
    emit this->inputsChanged();
}

void Node::setOutputs(QList<SharedNodeSocket> outputs) {
    m_outputs = outputs;
    emit this->outputsChanged();
}

void Node::setView(NodeView *view) { m_view = view; }

SharedNodeSocket Node::input(int id) {
    for (auto &socket : m_inputs) {
        if (socket->id() == id) return socket;
    }
    return nullptr;
}

SharedNodeSocket Node::output(int id) {
    for (auto &socket : m_outputs) {
        if (socket->id() == id) return socket;
    }
    return nullptr;
}

QQmlComponent *Node::delegate(QQmlEngine &engine) {
    static std::unique_ptr<QQmlComponent> delegate;
    if (!delegate)
        delegate =
            std::make_unique<QQmlComponent>(&engine, "qrc:/NodeDelegate.qml");
    return delegate.get();
}

QJsonObject Node::toJsonObject() const {
    QJsonObject nodeObject;
    nodeObject["id"] = QString::number(m_id);
    nodeObject["helper"] = true;
    nodeObject["x"] = static_cast<double>(m_x);
    nodeObject["y"] = static_cast<double>(m_y);
    return nodeObject;
}

}  // namespace diagram
}  // namespace scivi
