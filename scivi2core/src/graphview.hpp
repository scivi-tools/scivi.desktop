template <class Node_t>
void GraphView::insertNode(Node_t *node, QQmlComponent *nodeComponent) {
    if (nodeComponent == nullptr) {
        const auto engine = qmlEngine(this);
        if (engine != nullptr) nodeComponent = Node_t::delegate(*engine);
    }
    try {
        NodeView *nodeView = static_cast<NodeView *>(
            createFromComponent(nodeComponent, node, nullptr));
        nodeView->setNode(node);
        node->setView(nodeView);
        nodeView->setPosition(QPointF(static_cast<qreal>(node->x()),
                                      static_cast<qreal>(node->y())));
        QObject::connect(nodeView, &NodeView::inputClicked, this,
                         &GraphView::inputClicked);
        QObject::connect(nodeView, &NodeView::outputClicked, this,
                         &GraphView::outputClicked);
        QObject::connect(nodeView, &NodeView::inputEntered, this,
                         [=](QQuickItem *socketView) {
                             emit this->inputEntered(node, socketView);
                         });
        QObject::connect(nodeView, &NodeView::inputExited, this,
                         [=](QQuickItem *socketView) {
                             emit this->inputExited(node, socketView);
                         });
        QObject::connect(nodeView, &NodeView::outputEntered, this,
                         [=](QQuickItem *socketView) {
                             emit this->outputEntered(node, socketView);
                         });
        QObject::connect(nodeView, &NodeView::outputExited, this,
                         [=](QQuickItem *socketView) {
                             emit this->outputExited(node, socketView);
                         });
        QObject::connect(nodeView, &NodeView::deleteClicked, this,
                         &GraphView::nodeDeleteClicked);
        QObject::connect(nodeView, &NodeView::bodyClicked, this,
                         &GraphView::nodeClicked);
        for (const auto &inp : node->inputs()) {
            for (const auto &e : inp->joinedEdges()) {
                auto edgeView = e->view();
                if (edgeView) {
                    edgeView->invalidate();
                }
            }
        }
        for (const auto &inp : node->outputs()) {
            for (const auto &e : inp->joinedEdges()) {
                auto edgeView = e->view();
                if (edgeView) {
                    edgeView->invalidate();
                }
            }
        }
    } catch (...) {
        qWarning() << "Error while insering node";
    }
}

template <class Edge_t>
void GraphView::insertEdge(Edge_t *edge, QQmlComponent *edgeComponent) {
    if (edgeComponent == nullptr) {
        const auto engine = qmlEngine(this);
        if (engine != nullptr) edgeComponent = Edge_t::delegate(*engine);
    }
    try {
        EdgeView *edgeView = static_cast<EdgeView *>(
            createFromComponent(edgeComponent, nullptr, edge));
        edge->setView(edgeView);
        edgeView->setGraph(this);
        edgeView->invalidate();
        QObject::connect(edgeView, &EdgeView::rightButtonClicked, this,
                         &GraphView::splitEdge);
    } catch (...) {
        qWarning() << "Error while inserting edge";
    }
}
