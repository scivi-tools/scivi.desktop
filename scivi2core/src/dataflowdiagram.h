#ifndef DATAFLOWDIAGRAM_H
#define DATAFLOWDIAGRAM_H

#include <QObject>
#include <QSharedPointer>
#include <QStringListModel>
#include <QJsonObject>

namespace scivi {

namespace diagram {

class Node;
class Edge;
class NodeSocket;

using SharedNode = QSharedPointer<Node>;
using SharedEdge = QSharedPointer<Edge>;

class DataflowDiagram : public QObject
{
    Q_OBJECT
public:
    explicit DataflowDiagram(QString ontPath, QObject *parent = nullptr);
    explicit DataflowDiagram(QObject *parent = nullptr);
    ~DataflowDiagram();
    void insertNode(SharedNode node);
    void insertEdge(SharedEdge edge);
    void deleteNode(Node *node);
    void deleteEdge(const SharedEdge &edge);

    QList<SharedNode> &nodes();
    QList<SharedEdge> &edges();
    QList<SharedEdge> edgesFrom(const NodeSocket *sock);
    QList<SharedEdge> edgesTo(const NodeSocket *sock);
    SharedNode nodeById(int id) const;
    SharedEdge edge(int sourceSocketId, int destinationSocketId) const;
    int nextAvailableLinkNumber();
    QString pathToOntology() const;

    QJsonObject toJsonObject() const;

signals:
    void nodeInserted(SharedNode node);

private:
    QList<SharedNode> m_nodes;
    QList<SharedEdge> m_edges;
    QString m_ontPath;

    QList<SharedEdge> associatedEdges(int nodeId);
};

}

}

Q_DECLARE_METATYPE(scivi::diagram::DataflowDiagram*);

#endif // DATAFLOWDIAGRAM_H
