#include "connectionhandler.h"
#include "graphview.h"

namespace scivi {

namespace {



}

ConnectionHandler::ConnectionHandler(GraphView *graphView): m_graphView(graphView)
{
    QObject::connect(graphView, &GraphView::inputClicked, this, &ConnectionHandler::onInputClicked);
    QObject::connect(graphView, &GraphView::outputClicked, this, &ConnectionHandler::onOutputClicked);
    QObject::connect(graphView, &GraphView::inputEntered, this, &ConnectionHandler::onInputEntered);
    QObject::connect(graphView, &GraphView::spaceClicked, this, &ConnectionHandler::resetConnectionState);
}

bool ConnectionHandler::isActive() const
{
    return !m_tempEdge.isNull();
}

void ConnectionHandler::onInputClicked(NodeView *node, QQuickItem *socketView, NodeSocket *socket)
{
    Q_UNUSED(node);
    Q_UNUSED(socketView);
    Q_ASSERT(socket != nullptr);
    if (isActive()) {
        if (!isValidConnection(m_tempEdge->src(), socket)) return;
        auto sharedDestSocket = socket->node()->input(socket->id());
        m_tempEdge->setDest(sharedDestSocket);
        m_tempEdge->view()->invalidate();
        emit this->confirmEdge(m_tempEdge);
        resetConnectionState();
    } else {
       auto associatedEdges = m_graphView->diagram()->edgesTo(socket);
       // Detach accociated edge
       if (!associatedEdges.isEmpty()) {
            auto edge = associatedEdges.first();
            emit deleteEdge(edge);
       }
    }
}

void ConnectionHandler::onOutputClicked(NodeView *node, QQuickItem *socketView, NodeSocket *socket)
{
    Q_UNUSED(node);
    Q_UNUSED(socketView);
    Q_ASSERT(socket != nullptr);

    if (!isActive()) {
        m_tempEdge = QSharedPointer<Edge>::create();
        auto sharedSrc = socket->node()->output(socket->id());
        m_tempEdge->setSrc(sharedSrc);
        m_graphView->insertEdge<Edge>(m_tempEdge.data(), nullptr);
        auto edgeView = m_tempEdge->view();
        edgeView->setSource(node, socketView);
        edgeView->setSrcPointGlobal(calcSocketCenter(socketView));
        edgeView->setDestPointGlobal(calcSocketCenter(socketView));
        connectDestWithMousePosition(edgeView);
    }
}

void ConnectionHandler::onInputEntered(Node *node, QQuickItem *socketView)
{
    Q_UNUSED(node);
    if (m_tempEdge != nullptr)
        m_tempEdge->view()->setDestPointGlobal(calcSocketCenter(socketView));
}

void ConnectionHandler::resetConnectionState()
{
    disconnect(m_graphView, &GraphView::mousePositionChanged, this, 0);
    m_tempEdge.clear();
}

void ConnectionHandler::connectDestWithMousePosition(EdgeView *edgeView)
{
    connect(m_graphView, &GraphView::mousePositionChanged, this, [=](float x, float y) {
        edgeView->setDestPointGlobal(QPointF(x, y));
    });
}

bool ConnectionHandler::isValidConnection(NodeSocket *from, NodeSocket *to)
{
    return from->dataType() == to->dataType();
}

}
