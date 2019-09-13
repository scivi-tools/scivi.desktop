#include "edgeview.h"
#include "./graphview.h"
#include "./nodesocket.h"
#include "./edge.h"

namespace scivi {
namespace diagram {

EdgeView::EdgeView(QQuickItem *parent): QQuickItem(parent)
{

}

Edge *EdgeView::edge()
{
    return m_edge.data();
}

void EdgeView::setEdge(Edge *edge)
{
    m_edge = edge;
}

void EdgeView::setGraph(GraphView *graphView)
{
    m_graphView = graphView;
}

void EdgeView::setSource(NodeView *node, QQuickItem *socket)
{
    m_srcNode = node;
    m_srcSocket = socket;
}

void EdgeView::setDestination(NodeView *node, QQuickItem *socket)
{
    m_destNode = node;
    m_destSocket = socket;
}

QPointF EdgeView::srcPoint()
{
    return m_position.start;
}

void EdgeView::setSrcPoint(QPointF point)
{
    if (m_position.start != point) {
        m_position.start = point;
        updateControlPoints();
        emit srcPointChanged(point);
    }
}

void EdgeView::setSrcPointGlobal(QPointF point)
{
    setSrcPoint(m_graphView->mapFromGlobal(point));
}

QPointF EdgeView::destPoint()
{
    return m_position.end;
}

void EdgeView::setDestPoint(QPointF point)
{
    if (m_position.end != point) {
        m_position.end = point;
        updateControlPoints();
        emit destPointChanged(point);
    }
}

void EdgeView::setDestPointGlobal(QPointF point)
{
    setDestPoint(m_graphView->mapFromGlobal(point));
}

QPointF EdgeView::c1()
{
    return m_position.control1;
}

QPointF EdgeView::c2()
{
    return m_position.control2;
}

void EdgeView::switchToPortalView(int number)
{
    edge()->setNumber(number);
}

void EdgeView::switchToNormalView()
{
    edge()->setNumber(-1);
}

void connectSockets(QQuickItem *srcSocket, QQuickItem *destSocket, NodeView *srcNodeView,
                  NodeView *destNodeView, EdgeView *edgeView)
{
    Q_ASSERT(srcSocket != nullptr);
    Q_ASSERT(destSocket != nullptr);
    Q_ASSERT(srcNodeView != nullptr);
    Q_ASSERT(destNodeView != nullptr);
    Q_ASSERT(edgeView != nullptr);
    edgeView->setSource(srcNodeView, srcSocket);
    edgeView->setDestination(destNodeView, destSocket);
    auto updatePositionSlot = edgeView->metaObject()->method(edgeView->metaObject()->indexOfSlot("updatePosition()"));
    auto metaSourceNodeView = srcNodeView->metaObject();
    auto srcNodeX = metaSourceNodeView->property(metaSourceNodeView->indexOfProperty("x"));
    auto srcNodeY = metaSourceNodeView->property(metaSourceNodeView->indexOfProperty("y"));
    auto metaDestinationNodeView = destNodeView->metaObject();
    auto destNodeX = metaDestinationNodeView->property(metaDestinationNodeView->indexOfProperty("x"));
    auto destNodeY = metaDestinationNodeView->property(metaDestinationNodeView->indexOfProperty("y"));
    if (!destNodeX.isValid() || !destNodeX.hasNotifySignal()) {
        qWarning() << "Error can't access destinaiton node x property";
    }
    if (!destNodeY.isValid() || !destNodeY.hasNotifySignal()) {
        qWarning() << "Error can't access destinaiton node y property";
    }
    if (!srcNodeX.isValid() || !srcNodeX.hasNotifySignal()) {
        qWarning() << "Error can't access source node x property";
    }
    if (!srcNodeY.isValid() || !srcNodeY.hasNotifySignal()) {
        qWarning() << "Error can't access source node y property";
    }
    // Connect node changing with edge updating
    QObject::connect(srcNodeView, srcNodeX.notifySignal(), edgeView, updatePositionSlot);
    QObject::connect(srcNodeView, srcNodeY.notifySignal(), edgeView, updatePositionSlot);
    QObject::connect(destNodeView, destNodeX.notifySignal(), edgeView, updatePositionSlot);
    QObject::connect(destNodeView, destNodeY.notifySignal(), edgeView, updatePositionSlot);
}

void EdgeView::invalidate()
{
    if (!edge()->src() || !edge()->dest()) return;
    auto from = edge()->src();
    auto to = edge()->dest();
    auto srcNodeView = from->node()->view();
    auto destNodeView = to->node()->view();
    int srcSocketIndex = from->node()->outputs().indexOf(from->node()->output(from->id()));;
    int destSocketIndex = to->node()->inputs().indexOf(to->node()->input(to->id()));
    auto srcSocket = srcNodeView->getOutputSocket(srcSocketIndex);
    auto destSocket = destNodeView->getInputSocket(destSocketIndex);
    connectSockets(srcSocket, destSocket, srcNodeView, destNodeView, this);
    updatePosition();
}

QPointF EdgeView::getSourceSocketCenter()
{
    return m_srcSocket != nullptr ? calcSocketCenter(m_srcSocket) : QPointF();
}

QPointF EdgeView::getDestinationSocketCenter()
{
    return m_destSocket != nullptr ? calcSocketCenter(m_destSocket) : QPointF();
}

void EdgeView::updateControlPoints()
{
    auto src = m_position.start;
    auto dest = m_position.end;
    auto srcX = src.x();
    auto destX = dest.x();
    if (srcX > destX) {
        auto dx = srcX - destX;
        m_position.control1 = QPointF(dx + srcX, src.y());
        m_position.control2 = QPointF(destX-dx, dest.y());
    } else {
        auto sum = srcX + destX;
        m_position.control1 = QPointF(sum / 2, src.y());
        m_position.control2 = QPointF(sum / 2, dest.y());
    }
    emit this->controlPointsChanged();
}

void EdgeView::updatePosition()
{
    if (m_graphView == nullptr) return;
    QPointF globalSrcsocketPoint = getSourceSocketCenter();
    QPointF newSrcPoint = m_graphView->mapFromGlobal(globalSrcsocketPoint);
    setSrcPoint(newSrcPoint);
    QPointF globalDestsocketPoint = getDestinationSocketCenter();
    QPointF newDestPoint = m_graphView->mapFromGlobal(globalDestsocketPoint);
    setDestPoint(newDestPoint);
}

}
}
