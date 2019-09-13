#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QQmlComponent>
#include <QPointer>
#include <QSharedPointer>
#include <QColor>

namespace scivi {
namespace diagram {

class EdgeView;
class NodeSocket;

using SharedNodeSocket = QSharedPointer<NodeSocket>;

class Edge : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Edge)
    Q_PROPERTY(int number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(QColor color READ color CONSTANT FINAL)
    Q_PROPERTY(NodeSocket* src READ src NOTIFY srcChanged)
    Q_PROPERTY(NodeSocket* dest READ dest NOTIFY destChanged)

public:
    friend class EdgeView;
    explicit Edge();
    explicit Edge(SharedNodeSocket sourceSocket, SharedNodeSocket destSocket);
    ~Edge();

    EdgeView *view();
    void setView(EdgeView *view);
    void setSrc(SharedNodeSocket socket);
    void setDest(SharedNodeSocket socket);
    NodeSocket *src();
    NodeSocket *dest();
    void setNumber(int number);
    int number();
    QColor color();

    static QQmlComponent *delegate(QQmlEngine &engine);

signals:
    void numberChanged();
    void srcChanged(SharedNodeSocket);
    void destChanged(SharedNodeSocket);

private:
    int m_number = -1;
    SharedNodeSocket m_src;
    SharedNodeSocket m_dest;
    QPointer<EdgeView> m_view;
};

}
}
QML_DECLARE_TYPE(scivi::diagram::Edge)

#endif // EDGE_H
