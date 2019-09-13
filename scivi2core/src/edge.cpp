#include "edge.h"

#include <memory>
#include "./edgeview.h"
#include "./node.h"

namespace scivi {
namespace diagram {

Edge::Edge(): QObject(nullptr), m_src(nullptr), m_dest(nullptr)
{

}

Edge::Edge(SharedNodeSocket sourceSocket, SharedNodeSocket destSocket)
    : QObject(nullptr), m_src(sourceSocket), m_dest(destSocket)
{
}

EdgeView *Edge::view()
{
    return m_view.data();
}

void Edge::setView(EdgeView *view)
{
    m_view = view;
}

void Edge::setSrc(SharedNodeSocket socket)
{
    m_src = socket;
    emit srcChanged(socket);
}

void Edge::setDest(SharedNodeSocket socket)
{
    m_dest = socket;
    emit destChanged(socket);
}

NodeSocket* Edge::src()
{
    return m_src.data();
}

NodeSocket* Edge::dest()
{
    return m_dest.data();
}

void Edge::setNumber(int number)
{
    m_number = number;
    emit numberChanged();
}

int Edge::number()
{
    return m_number;
}

QColor Edge::color()
{
    return m_src != nullptr ? m_src->color() : QColor();
}

Edge::~Edge()
{
    if (!m_view.isNull())
        m_view->deleteLater();
    if (!m_src.isNull()) {
        m_src.clear();
    }
    if (!m_dest.isNull()) {
        m_dest.clear();
    }
}

QQmlComponent *Edge::delegate(QQmlEngine &engine)
{
    static std::unique_ptr<QQmlComponent> delegate;
    if (!delegate)
        delegate = std::make_unique<QQmlComponent>(&engine, "qrc:/EdgeDelegate.qml");
    return delegate.get();
}


}
}
