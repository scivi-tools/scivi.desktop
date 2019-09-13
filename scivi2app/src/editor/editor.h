#ifndef EDITOR_H
#define EDITOR_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QQuickItem>
#include <QSharedPointer>

#include <edge.h>
#include <nodeview.h>

#include "connectionhandler.h"
#include "graphview.h"
#include "knowledge/knowledgeservice.h"
#include "palette/palettefilterproxy.h"
#include "palette/palettemodel.h"

#include "interpreter/interpreter.h"
#include "nodegroup.h"
#include "typesstore.h"

using namespace scivi::palette;
using namespace scivi::knowledge;
using namespace scivi::diagram;

namespace scivi {

using SharedDiagram = QSharedPointer<DataflowDiagram>;

class Editor : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QObject *paletteModel READ paletteModelProp NOTIFY paletteModelChanged)
    Q_PROPERTY(bool isConnecting READ isConnecting)
    Q_PROPERTY(QObject *selection READ selection NOTIFY selectedNodesChanged)
    Q_PROPERTY(
        QObject *selectedNode READ selectedNode NOTIFY selectedNodesChanged)
    Q_PROPERTY(DataflowDiagram *diagram READ diagram)
    Q_PROPERTY(QString ontPath READ ontPath NOTIFY ontologyLoaded)
    Q_PROPERTY(QString diagramPath READ diagramPath NOTIFY diagramPathChanged)

public:
    explicit Editor(QQmlEngine *engine, QObject *parent = nullptr);

    QObject *paletteModelProp();
    Q_INVOKABLE void setupFromFile(QString fileUrl);
    Q_INVOKABLE void setGraphView(GraphView *graphView);
    Q_INVOKABLE void exportDiagram(QString fileUrl);
    Q_INVOKABLE void importDiagramFrom(QString fileUrl);
    Q_INVOKABLE void combineSelectedNodes(QString nodeName);
    bool isConnecting() const;
    void resetSelection();
    QObject *selection();
    QObject *selectedNode();
    DataflowDiagram *diagram() const;
    QString ontPath() const;
    Q_INVOKABLE void visualize(QQuickItem *container);
    QString diagramPath() const;

public slots:
    bool paletteItemClicked(QModelIndex index, int depth);
    void deleteNode(NodeView *view);
    void deleteNodeModel(Node *node);
    void addEdgeToDiagram(QSharedPointer<Edge> edge);
    void nodeClicked(NodeView *nodeView, int modifiers);
    void deselectNode(NodeView *nodeView);
    void deleteEdgeFromDiagram(QSharedPointer<Edge> edge);

signals:
    void ontologyLoaded();
    void diagramPathChanged();
    void paletteModelChanged();
    void selectedNodesChanged();
    void nodeAdded(QObject *);

private:
    KnowledgeService m_knowledgeService;
    PaletteModel m_paletteModel;
    PaletteFilterProxy m_paletteProxyModel;
    SharedDiagram m_diagram;
    TypesStore m_typesStore;
    diagram::GraphView *m_graphView = {nullptr};
    NodeGroup m_nodeSelection;
    QSharedPointer<ConnectionHandler> m_connectionHandler;
    QSharedPointer<Interpreter> m_interpreter;
    QString m_diagramPath;

    void buildPalette();

    QQmlEngine *m_engine = {nullptr};
};

}  // namespace scivi

#endif  // EDITOR_H
