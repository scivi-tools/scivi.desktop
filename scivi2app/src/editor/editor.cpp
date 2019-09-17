#include "editor.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QUrl>
#include <QVariantMap>

#include <dataflowdiagram.h>
#include <datanode.h>
#include <ont/ontology.h>
#include <utils/path.h>
#include "palette/paletteitem.h"

#include "palette/palettebuilder.h"

#include <combineddatanode.h>
#include <nodesocket.h>
#include "interpreter/interpreter.h"
#include "io/diagramwriter.h"
#include "knowledge/concept.h"
#include "knowledge/reader.h"
#include "nodefactory.h"

using namespace scivi::knowledge;

namespace scivi {

Editor::Editor(QQmlEngine *engine, QObject *parent)
    : QObject(parent),
      m_paletteProxyModel(&m_knowledgeService, this),
      m_diagram(SharedDiagram(new DataflowDiagram(this))),
      m_engine(engine) {
    m_paletteProxyModel.setSourceModel(&m_paletteModel);
    QObject::connect(&m_nodeSelection, &NodeGroup::changed, this,
                     &Editor::selectedNodesChanged);
    QObject::connect(&m_nodeSelection, &NodeGroup::added, this,
                     [&](NodeView *nodeView) {
                         if (nodeView != nullptr) {
                             nodeView->setSelected(true);
                         }
                     });
    QObject::connect(&m_nodeSelection, &NodeGroup::removed, this,
                     [&](NodeView *nodeView) {
                         if (nodeView != nullptr) {
                             nodeView->setSelected(false);
                         }
                     });
    QObject::connect(this, &Editor::selectedNodesChanged, this, [&]() {
        auto selectedNode = this->selectedNode();
        if (selectedNode != nullptr) {
            auto selectedNodeView =
                reinterpret_cast<Node *>(selectedNode)->view();
            m_paletteProxyModel.setSelectedNode(selectedNodeView);
        } else {
            m_paletteProxyModel.setSelectedNode(nullptr);
        }
    });
}

QObject *Editor::paletteModelProp() {
    return reinterpret_cast<QObject *>(&m_paletteProxyModel);
}

void Editor::setupFromFile(QString fileUrl) {
    auto ontology = Reader::readFromFile(fileUrl);
    if (ontology == nullptr) {
        qWarning("Couldn't open ontology file");
        return;
    }
    m_knowledgeService = KnowledgeService(*ontology);
    m_diagram = SharedDiagram::create(fileUrl, this);
    if (m_graphView != nullptr) {
        m_graphView->setDiagram(m_diagram.data());
    }
    m_typesStore = TypesStore(&m_knowledgeService, m_engine);
    buildPalette();
    m_diagramPath = "";
    emit diagramPathChanged();
    emit ontologyLoaded();
}

void Editor::setGraphView(diagram::GraphView *graphView) {
    m_graphView = graphView;
    m_graphView->setDiagram(m_diagram.data());
    m_connectionHandler = QSharedPointer<ConnectionHandler>::create(graphView);
    connect(m_connectionHandler.data(), &ConnectionHandler::confirmEdge, this,
            &Editor::addEdgeToDiagram);
    connect(m_connectionHandler.data(), &ConnectionHandler::deleteEdge, this,
            &Editor::deleteEdgeFromDiagram);
    connect(m_graphView, &GraphView::nodeClicked, this, &Editor::nodeClicked);
    connect(m_graphView, &GraphView::spaceClicked, this,
            &Editor::resetSelection);
    connect(m_graphView, &GraphView::nodeDeleteClicked, this,
            &Editor::deleteNode);
}

void Editor::exportDiagram(QString fileUrl) {
    auto json = m_diagram->toJsonObject();
    m_diagramPath = fileUrl;
    if (writeToFile(json, fileUrl)) {
        qInfo() << "Successfully saved diagram to " << fileUrl;
        emit diagramPathChanged();
    }
}

void Editor::importDiagramFrom(QString fileUrl) {
    auto importResult = readFromFile(fileUrl, m_engine);
    if (!importResult.isNull()) {
        setupFromFile(importResult->pathToOntology());
        m_diagram = importResult;
        m_diagramPath = fileUrl;
        emit ontologyLoaded();
        emit diagramPathChanged();
        if (m_graphView != nullptr) {
            m_graphView->setDiagram(m_diagram.data());
            qInfo() << "Successfully read diagram from file " << fileUrl;
            qDebug() << "Ontology file is " << importResult->pathToOntology();
        }
    }
}

void Editor::combineSelectedNodes(QString nodeName) {
    qDebug() << "Combine selected nodes";
    auto nodeViews = m_nodeSelection.nodes();
    QList<DataNode *> nodes;
    for (const auto &nodeView : nodeViews) {
        DataNode *n = dynamic_cast<DataNode *>(nodeView->node());
        if (n == nullptr) {
            continue;
        }
        nodes.append(n);
    }
    auto combinedNode =
        QSharedPointer<CombinedDataNode>::create(nodeName, nodes);
    m_diagram->insertNode(combinedNode);
    for (const auto &node : nodes) {
        node->view()->setVisible(false);
    }
}

bool Editor::isConnecting() const {
    return m_connectionHandler.isNull() ? false
                                        : m_connectionHandler->isActive();
}

void Editor::resetSelection() { m_nodeSelection.clear(); }

QObject *Editor::selection() {
    return reinterpret_cast<QObject *>(&m_nodeSelection);
}

QObject *Editor::selectedNode() {
    if (m_nodeSelection.isEmpty() || m_nodeSelection.isMulti()) {
        return nullptr;
    }
    return qobject_cast<QObject *>(m_nodeSelection.at(0)->node());
}

DataflowDiagram *Editor::diagram() const {
    this->m_diagram.data()->setObjectName("Diagram");
    return this->m_diagram.data();
}

QString Editor::ontPath() const {
    return m_diagram ? m_diagram->pathToOntology() : "";
}

void Editor::visualize(QQuickItem *container) {
    if (container == nullptr) {
        qWarning() << "Container for visualization is empty";
        return;
    }
    QDir dir(qApp->applicationDirPath());
    if (!m_interpreter.isNull()) {
        m_interpreter->deleteLater();
        m_interpreter = nullptr;
    }
    m_interpreter = QSharedPointer<Interpreter>::create(this);
    m_interpreter->interpret(this->m_diagram.data(), container);
}

QString Editor::diagramPath() const { return m_diagramPath; }

bool Editor::paletteItemClicked(QModelIndex index, int depth) {
    if (depth == 0) return false;
    auto clickedPaletteItemId = m_paletteModel.getId(index);
    if (m_graphView != nullptr) {
        auto node =
            createNode(clickedPaletteItemId, m_knowledgeService, m_typesStore);
        m_diagram->insertNode(node);
    } else {
        qWarning() << "Graph view is null when inserting new node";
        return false;
    }
    return true;
}

void Editor::deleteNode(NodeView *view) { m_diagram->deleteNode(view->node()); }

void Editor::deleteNodeModel(Node *node) { m_diagram->deleteNode(node); }

void Editor::addEdgeToDiagram(QSharedPointer<Edge> edge) {
    auto presentEdges = m_diagram->edgesTo(edge->dest());
    for (auto &edge : presentEdges) {
        m_diagram->deleteEdge(edge);
    }
    m_diagram->insertEdge(edge);
}

void Editor::nodeClicked(NodeView *nodeView, int modifiers) {
    bool withShift = static_cast<unsigned int>(modifiers) & Qt::ShiftModifier;
    bool isSelected = m_nodeSelection.isNodeSelected(nodeView);

    if (isSelected && withShift) {
        m_nodeSelection.remove(nodeView);
    } else if (!isSelected) {
        if (!withShift) {
            m_nodeSelection.clear();
        }
        m_nodeSelection.add(nodeView);
    }
}

void Editor::deselectNode(NodeView *nodeView) {
    m_nodeSelection.remove(nodeView);
}

void Editor::deleteEdgeFromDiagram(QSharedPointer<Edge> edge) {
    m_diagram->deleteEdge(edge);
}

void Editor::buildPalette() {
    auto rootConcept = m_knowledgeService.conceptByName("Root");
    auto rootConcepts = m_knowledgeService.flatChildrenOf(rootConcept);
    PaletteBuilder paletteBuilder(&m_knowledgeService);
    auto paletteConcepts = paletteBuilder.fromConcepts(rootConcepts);
    m_paletteModel.setupModel(paletteConcepts);
    for (auto p : paletteConcepts) {
        delete p;
    }
    emit this->paletteModelChanged();
}

}  // namespace scivi
