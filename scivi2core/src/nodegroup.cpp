#include "nodegroup.h"
#include "nodeview.h"

namespace scivi {
namespace diagram {

NodeGroup::NodeGroup(QObject *parent) : QObject(parent) {}

NodeGroup::~NodeGroup() { clear(); }

bool NodeGroup::isNodeSelected(NodeView *nodeView) const {
    // TODO: Compare by id?
    return m_nodes.contains(nodeView);
}

void NodeGroup::add(NodeView *nodeView) {
    m_nodes.append(nodeView);
    QObject::connect(nodeView, &NodeView::destroyed, this, [&](QObject *obj) {
        this->remove(dynamic_cast<NodeView *>(obj));
    });
    emit added(nodeView);
    emit changed();
}

QList<NodeView *> NodeGroup::nodes() const { return m_nodes; }

void NodeGroup::remove(NodeView *nodeView) {
    m_nodes.removeOne(nodeView);
    emit removed(nodeView);
    emit changed();
}

void NodeGroup::clear() {
    for (const auto &node : m_nodes) {
        emit removed(node);
    }
    m_nodes.clear();
    emit changed();
}

NodeView *NodeGroup::at(int index) const { return m_nodes.at(index); }

bool NodeGroup::isMulti() const { return m_nodes.size() > 1; }

bool NodeGroup::isEmpty() const { return m_nodes.isEmpty(); }

}  // namespace diagram
}  // namespace scivi
