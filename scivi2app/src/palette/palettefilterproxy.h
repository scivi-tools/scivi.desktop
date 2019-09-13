#ifndef PALETTEFILTERPROXY_H
#define PALETTEFILTERPROXY_H

#include <QSortFilterProxyModel>

#include <nodeview.h>
#include "knowledge/knowledgeservice.h"

using namespace scivi::diagram;
using namespace scivi::knowledge;

class PaletteFilterProxy : public QSortFilterProxyModel {
    Q_OBJECT
public:
    PaletteFilterProxy(KnowledgeService *knowledgeService,
                       QObject *parent = nullptr);
    void setSelectedNode(NodeView *node);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const override;
    bool filterAcceptsRowItself(int source_row,
                                const QModelIndex &source_parent) const;
    bool hasAcceptedChildren(int source_row,
                             const QModelIndex &source_parent) const;

    bool isCompatibleWithSelected(int conceptId) const;

    QPointer<NodeView> m_selectedNode;
    KnowledgeService *m_knowledgeService;
};

#endif  // PALETTEFILTERPROXY_H
