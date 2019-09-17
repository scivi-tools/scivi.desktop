#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <QObject>
#include <QPointer>
#include <QQuickItem>
#include <QSharedPointer>

#include "graphview.h"

using namespace scivi::diagram;

namespace scivi {

class ConnectionHandler : public QObject {
    Q_OBJECT
public:
    ConnectionHandler(GraphView *graphView);
    bool isActive() const;

signals:
    void showEdge(SharedEdge edge);
    void confirmEdge(SharedEdge edge);
    void deleteEdge(SharedEdge edge);
    void cutEdge(SharedEdge edge);

public slots:
    void onInputClicked(NodeView *node, QQuickItem *socketView,
                        NodeSocket *socket);
    void onOutputClicked(NodeView *node, QQuickItem *socketView,
                         NodeSocket *socket);
    void onInputEntered(Node *node, QQuickItem *socketView);
    void resetConnectionState();

private:
    void connectDestWithMousePosition(EdgeView *edgeView);
    bool isValidConnection(NodeSocket *from, NodeSocket *to);

    GraphView *m_graphView;
    QSharedPointer<Edge> m_tempEdge;
};

}  // namespace scivi

#endif  // CONNECTION_HANDLER_H
