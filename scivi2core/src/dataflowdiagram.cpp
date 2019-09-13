#include "dataflowdiagram.h"

#include "node.h"
#include "edge.h"
#include "nodesocket.h"
#include <QJsonArray>
#include <utils/path.h>

namespace scivi {

namespace diagram {

DataflowDiagram::DataflowDiagram(QString ontPath, QObject *parent): QObject(parent), m_ontPath(ontPath)
{

}

DataflowDiagram::DataflowDiagram(QObject *parent) : QObject(parent)
{

}

DataflowDiagram::~DataflowDiagram() {
    // TODO: DRY
    for (const auto &edge: m_edges) {
        edge->src()->disjoinEdge(edge);
        edge->dest()->disjoinEdge(edge);
    }
    m_edges.clear();
    m_nodes.clear();
}

void DataflowDiagram::insertNode(SharedNode node)
{
    if (node != nullptr) {
        m_nodes.append(node);
        emit nodeInserted(node);
    }
}

void DataflowDiagram::insertEdge(SharedEdge edge)
{
    if (edge != nullptr) {
        m_edges.append(edge);
        edge->src()->joinEdge(edge);
        edge->dest()->joinEdge(edge);
    }
}

void DataflowDiagram::deleteNode(Node *node)
{
    // Delete all accociated edges
    auto edges = associatedEdges(node->id());
    for (auto &edge: edges) {
        deleteEdge(edge);
    }
    // Delete node
    auto sharedNode = nodeById(node->id());
    m_nodes.removeAll(sharedNode);
}

void DataflowDiagram::deleteEdge(const SharedEdge &edge)
{
    edge->src()->disjoinEdge(edge);
    edge->dest()->disjoinEdge(edge);
    m_edges.removeAll(edge);
}

QList<SharedNode> &DataflowDiagram::nodes()
{
    return m_nodes;
}

QList<SharedEdge> &DataflowDiagram::edges()
{
    return m_edges;
}

QList<SharedEdge> DataflowDiagram::edgesFrom(const NodeSocket *sock)
{
    QList<SharedEdge> res;
    for (auto &edge : m_edges) {
        if (edge->src()->id() == sock->id()) {
            res << edge;
        }
    }
    return res;
}

QList<SharedEdge> DataflowDiagram::edgesTo(const NodeSocket *sock)
{
    QList<SharedEdge> res;
    for (auto &edge : m_edges) {
        if (edge->dest() == sock) {
            res << edge;
        }
    }
    return res;
}

SharedNode DataflowDiagram::nodeById(int id) const
{
    foreach (auto node, m_nodes) {
        if (node->id() == id)
            return node;
    }
    return nullptr;
}

SharedEdge DataflowDiagram::edge(int sourceSocketId, int destinationSocketId) const
{
    QList<SharedEdge>::const_iterator it = std::find_if(m_edges.constBegin(), m_edges.constEnd(), [=](SharedEdge edge) {
        return edge->src()->id() == sourceSocketId && edge->dest()->id() == destinationSocketId;
    });
    return it != nullptr ? *it : nullptr;
}

int DataflowDiagram::nextAvailableLinkNumber()
{
    QSet<int> settedNumbers;
    for (auto &edge: m_edges) {
        if (edge->number() != -1)
            settedNumbers << edge->number();
    }
    QSet<int> allNumbers;
    for (int i = 0; i < m_edges.length(); i++) {
        allNumbers << i+1;
    }
    int minNumber = m_edges.length() + 1;
    for (auto &num: allNumbers.subtract(settedNumbers)) {
        if (num < minNumber) {
            minNumber = num;
        }
    }
    return minNumber;
}

QString DataflowDiagram::pathToOntology() const {
    return m_ontPath;
}

QJsonObject DataflowDiagram::toJsonObject() const
{
    QJsonObject diagramObject;
    QJsonArray nodesArray;
    QMap<int, int> nodeIdToIndexMap;
    int i = 0;
    for(const auto &node : m_nodes) {
        nodesArray.append(node->toJsonObject());
        nodeIdToIndexMap.insert(node->id(), i);
        i++;
    }
    QJsonArray linksArray;
    for(auto &edge: m_edges) {
        QJsonObject link;
        link["node_from"] = nodeIdToIndexMap.value(edge->src()->node()->id());
        link["node_to"] = nodeIdToIndexMap.value(edge->dest()->node()->id());
        link["socket_from"] = QString::number(edge->src()->id());
        link["socket_to"] = QString::number(edge->dest()->id());
        if (edge->number() != -1)
            link["number"] = edge->number();
        linksArray.append(link);
    }
    diagramObject["onto"] = PathUtils::removeSchema(m_ontPath);
    diagramObject["nodes"] = nodesArray;
    diagramObject["links"] = linksArray;
    return diagramObject;
}

QList<SharedEdge> DataflowDiagram::associatedEdges(int nodeId)
{
    QList<SharedEdge> edges;
    for (auto &edge: m_edges) {
        if (edge->src()->node()->id() == nodeId || edge->dest()->node()->id() == nodeId) {
            edges << edge;
        }
    }
    return edges;
}

}
}
