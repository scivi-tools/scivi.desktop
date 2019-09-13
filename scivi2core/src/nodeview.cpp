#include "nodeview.h"

namespace scivi {
namespace diagram {

QPointF calcSocketCenter(QQuickItem *socketView) {
    qreal x = static_cast<qreal>(socketView->x());
    qreal y = static_cast<qreal>(socketView->y());
    qreal width = static_cast<qreal>(socketView->width());
    qreal height = static_cast<qreal>(socketView->height());
    auto socketParent = socketView->parentItem();
    if (socketParent == nullptr || !qIsFinite(x) || !qIsFinite(y))
        return QPointF();
    return socketParent->mapToGlobal(QPointF(x + width / 2, y + height / 2));
}

NodeView::NodeView(QQuickItem *parent) : QQuickItem(parent) {}

Node *NodeView::node() { return m_node.data(); }

void NodeView::setNode(Node *node) { m_node = node; }

QQuickItem *NodeView::getInputSocket(int index) {
    return m_inputSockets.at(index);
}

QQuickItem *NodeView::getOutputSocket(int index) {
    return m_outputSockets.at(index);
}

bool NodeView::isSelected() const { return m_isSelected; }

void NodeView::setSelected(bool isSelected) {
    m_isSelected = isSelected;
    emit this->selectedChanged(isSelected);
}

void NodeView::inputCreated(QQuickItem *socketView) {
    m_inputSockets.append(socketView);
}

void NodeView::outputCreated(QQuickItem *socketView) {
    m_outputSockets.append(socketView);
}

}  // namespace diagram
}  // namespace scivi
