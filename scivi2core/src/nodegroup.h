#ifndef NODEGROUP_H
#define NODEGROUP_H

#include <QObject>

namespace scivi {
namespace diagram {

class NodeView;

class NodeGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY changed)
    Q_PROPERTY(bool isMulti READ isMulti NOTIFY changed)

public:
    explicit NodeGroup(QObject *parent = nullptr);
    ~NodeGroup();
    bool isNodeSelected(NodeView *nodeView) const;
    void clear();
    void remove(NodeView *nodeView);
    NodeView* at(int index) const;
    bool isMulti() const;
    bool isEmpty() const;
    void add(NodeView *nodeView);
    QList<NodeView*> nodes() const;

signals:
    void changed();
    void added(NodeView *nodeView);
    void removed(NodeView *nodeView);

private:
    QList<NodeView*> m_nodes;
};

}
}

#endif // NODEGROUP_H
