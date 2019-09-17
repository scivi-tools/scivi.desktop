#include "./graphview.h"

#include <datanode.h>
#include <QQmlEngine>
#include <memory>
#include "dataflowdiagram.h"
#include "edge.h"
#include "node.h"
#include "nodeview.h"
#include "utils/error.h"

namespace scivi {
namespace diagram {

GraphView::GraphView(QQuickItem *parent) : QQuickItem(parent) {}

void GraphView::setDiagram(DataflowDiagram *diagram) {
    m_diagram = diagram;
    for (auto &node : m_diagram->nodes()) {
        auto dataNode = qSharedPointerObjectCast<DataNode>(node);
        if (dataNode != nullptr) {  // TODO: Add more node types
            insertNode<DataNode>(dataNode.data(), nullptr);
        }
    }
    for (auto &edge : m_diagram->edges()) {
        insertEdge<Edge>(edge.data(), nullptr);
    }
    connect(diagram, &diagram::DataflowDiagram::nodeInserted, this,
            &GraphView::onNodeInserted);
}

DataflowDiagram *GraphView::diagram() { return m_diagram; }

void GraphView::splitEdge(EdgeView *view) {
    auto edgesFromSrc = m_diagram->edgesFrom(view->edge()->src());
    // Check is portal node already placed
    for (auto &edge : edgesFromSrc) {
        if (edge->number() != -1) return;
    }
    auto portalLinkNumber = m_diagram->nextAvailableLinkNumber();
    view->switchToPortalView(portalLinkNumber);
}

void GraphView::onNodeInserted(SharedNode node) {
    // TODO: More types of data nodes
    const auto dataNode = qSharedPointerObjectCast<DataNode>(node);
    insertNode<DataNode>(dataNode.data(), nullptr);
}

std::unique_ptr<QQmlComponent> GraphView::createComponent(const QString &url) {
    if (url.isEmpty()) {
        qWarning() << "createComponent empty URL";
        return std::unique_ptr<QQmlComponent>();
    }

    QQmlEngine *engine = qmlEngine(this);
    std::unique_ptr<QQmlComponent> component;
    if (engine != nullptr) {
        try {
            component = std::make_unique<QQmlComponent>(engine, url);
            if (!component->isReady() || component->isError() ||
                component->isNull()) {
                qWarning() << "Error while creating component for url " << url;
            }
            return component;
        } catch (...) {
            qWarning() << "Error while creating component";
        }
    } else {
        qWarning() << "No access to qml engine";
    }
    return component;
}

QQuickItem *GraphView::createFromComponent(QQmlComponent *component, Node *node,
                                           Edge *edge) {
    if (component == nullptr) {
        qWarning() << "crateFromComponent component is null";
        return nullptr;
    }
    QQuickItem *item = nullptr;
    try {
        if (!component->isReady())
            throw Error{"Error node delegate is not ready"};
        const auto rootContext = qmlContext(this);
        if (rootContext == nullptr)
            throw Error{"Error can't access local QML context"};
        QObject *object = component->beginCreate(rootContext);
        if (object == nullptr || component->isError()) {
            if (object != nullptr) object->deleteLater();
            throw Error{
                "Failed create concrete QQuickItem from QML Component " +
                component->errorString()};
        }
        if (node != nullptr) {
            const auto nodeView = qobject_cast<NodeView *>(object);
            if (nodeView != nullptr) {
                nodeView->setNode(node);
            }
        } else if (edge != nullptr) {
            const auto edgeView = qobject_cast<EdgeView *>(object);
            if (edgeView != nullptr) {
                edgeView->setEdge(edge);
            }
        }
        component->completeCreate();
        if (!component->isError()) {
            QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
            item = qobject_cast<QQuickItem *>(object);
            item->setVisible(true);
            item->setParentItem(this);
        }
    } catch (Error &e) {
        qWarning() << "Error while creating component " << e.msg();
    }
    return item;
}

}  // namespace diagram
}  // namespace scivi
