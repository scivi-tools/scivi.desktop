#ifndef EDGEVIEW_H
#define EDGEVIEW_H

#include <QPointer>
#include <QQuickItem>

#include "./edge.h"

namespace scivi {
namespace diagram {

class NodeView;
class GraphView;

struct EdgePosition {
    QPointF start;
    QPointF end;
    QPointF control1;
    QPointF control2;
};

class EdgeView : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Edge *edge READ edge CONSTANT FINAL)
    Q_PROPERTY(QPointF srcPoint READ srcPoint NOTIFY srcPointChanged)
    Q_PROPERTY(QPointF destPoint READ destPoint NOTIFY destPointChanged)
    Q_PROPERTY(QPointF c1 READ c1 NOTIFY controlPointsChanged)
    Q_PROPERTY(QPointF c2 READ c2 NOTIFY controlPointsChanged)

public:
    explicit EdgeView(QQuickItem *parent = nullptr);
    Edge *edge();
    void setEdge(Edge *edge);
    void setGraph(GraphView *graphView);
    void setSource(NodeView *node, QQuickItem *socket);
    void setDestination(NodeView *node, QQuickItem *socket);
    QPointF srcPoint();
    QQuickItem *srcSocket() { return m_srcSocket; }
    void setSrcPoint(QPointF point);
    void setSrcPointGlobal(QPointF point);
    QPointF destPoint();
    QQuickItem *destSocket() { return m_destSocket; }
    void setDestPoint(QPointF point);
    void setDestPointGlobal(QPointF point);
    QPointF c1();
    QPointF c2();
    void switchToPortalView(int number);
    void switchToNormalView();
    QColor color();
    void invalidate();

signals:
    void srcPointChanged(QPointF);
    void destPointChanged(QPointF);
    void controlPointsChanged();
    void rightButtonClicked(EdgeView *);

public slots:
    void updatePosition();

private:
    QPointer<Edge> m_edge{nullptr};
    NodeView *m_srcNode;
    QQuickItem *m_srcSocket{nullptr};
    NodeView *m_destNode;
    QQuickItem *m_destSocket{nullptr};
    GraphView *m_graphView;
    EdgePosition m_position;

    QPointF getSourceSocketCenter();
    QPointF getDestinationSocketCenter();
    void updateControlPoints();
};

}  // namespace diagram
}  // namespace scivi

QML_DECLARE_TYPE(scivi::diagram::EdgeView)

#endif  // EDGEVIEW_H
