#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <memory>
#include <QtQuick/QQuickItem>
#include <QQmlEngine>

#include "nodeview.h"
#include "edgeview.h"
#include "dataflowdiagram.h"

namespace scivi {
namespace diagram {

class GraphView : public QQuickItem
{
    Q_OBJECT

public:

    explicit GraphView(QQuickItem *parent = nullptr);
    template<class Node_t>
    void insertNode(Node_t *node, QQmlComponent *nodeComponent = nullptr);
    template<class Edge_t>
    void insertEdge(Edge_t *edge, QQmlComponent *edgeComponent);
    void setDiagram(DataflowDiagram *diagram);
    DataflowDiagram *diagram();

signals:
    void inputClicked(NodeView *node, QQuickItem *socketView, NodeSocket *socket);
    void outputClicked(NodeView *node, QQuickItem *socketView, NodeSocket *socket);
    void inputEntered(Node *node, QQuickItem *socketView);
    void inputExited(Node *node, QQuickItem *socketView);
    void outputEntered(Node *node, QQuickItem *socketView);
    void outputExited(Node *node, QQuickItem *socketView);
    void spaceClicked();
    void mousePositionChanged(float x, float y);
    void nodeClicked(NodeView *nodeView, int modifiers);
    void nodeDeleteClicked(NodeView *view);

public slots:
    void splitEdge(EdgeView *view);
    void onNodeInserted(SharedNode node);

private:
    QPointer<DataflowDiagram> m_diagram;

    std::unique_ptr<QQmlComponent> createComponent(const QString &url);
    QQuickItem *createFromComponent(QQmlComponent *component, Node *node, Edge *edge);
};

#include "./graphview.hpp"

}
}

#endif // GRAPH_VIEW_H
