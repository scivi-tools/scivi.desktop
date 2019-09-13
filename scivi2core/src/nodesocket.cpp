#include "nodesocket.h"
#include "./node.h"

namespace scivi {
namespace diagram {

NodeSocket::NodeSocket(int id, const QString &label, Type type, const NodeSocketType &dataType, Node *node, QObject *parent)
    : QObject(parent), m_id(id), m_label(label), m_type(type), m_dataType(dataType), m_node(node)
{

}

NodeSocket::NodeSocket(const NodeSocket &socket, Node *node, QObject *parent)
    : QObject(parent), m_id(socket.m_id), m_label(socket.m_label), m_type(socket.m_type),
      m_dataType(socket.m_dataType), m_node(node)
{

}

int NodeSocket::id() const
{
    return m_id;
}

QString NodeSocket::label() const
{
    return m_label;
}

QColor NodeSocket::color() const
{
    return m_dataType.color;
}

NodeSocketType NodeSocket::dataType()
{
    return m_dataType;
}

Node *NodeSocket::node()
{
    return m_node;
}

void NodeSocket::joinEdge(QSharedPointer<Edge> edge)
{
    m_connectedEdges.append(edge);
}

void NodeSocket::disjoinEdge(QSharedPointer<Edge> edge)
{
    m_connectedEdges.removeAll(edge);
}

bool NodeSocket::isJoined() const
{
    return !m_connectedEdges.isEmpty();
}

QList<QSharedPointer<Edge>> NodeSocket::joinedEdges() const
{
    return m_connectedEdges;
}

}
}
