#ifndef NODESOCKET_H
#define NODESOCKET_H

#include <QColor>
#include <QObject>
#include <QPointer>
#include <QString>

namespace scivi {
namespace diagram {

class Node;
class Edge;

struct NodeSocketType {
    QString name;
    QColor color;

    NodeSocketType(const QString &name, const QColor &color) {
        this->name = name;
        this->color = color;
    }

    bool operator==(const NodeSocketType &other) {
        return this->name == other.name;
    }
};

class NodeSocket : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString label READ label CONSTANT FINAL)
    Q_PROPERTY(bool isInput READ isInput CONSTANT FINAL)
    Q_PROPERTY(QColor color READ color CONSTANT FINAL)
    Q_PROPERTY(Node *node READ node)

    friend class Node;

public:
    enum Type { In, Out };

    explicit NodeSocket(int id, const QString &label, Type type,
                        const NodeSocketType &dataType, Node *node,
                        QObject *parent = nullptr);
    explicit NodeSocket(const NodeSocket &socket, Node *node,
                        QObject *parent = nullptr);
    bool isInput() { return m_type == Type::In; }
    int id() const;
    QString label() const;
    QColor color() const;
    NodeSocketType dataType();
    Node *node();
    void joinEdge(QSharedPointer<Edge> edge);
    void disjoinEdge(QSharedPointer<Edge> edge);
    bool isJoined() const;
    QList<QSharedPointer<Edge>> joinedEdges() const;

private:
    int m_id;
    QString m_label;
    Type m_type;
    NodeSocketType m_dataType;
    Node *m_node;
    QList<QSharedPointer<Edge>> m_connectedEdges;
};

}  // namespace diagram
}  // namespace scivi

Q_DECLARE_METATYPE(scivi::diagram::NodeSocket *)

#endif  // NODESOCKET_H
