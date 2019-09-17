#include "palettefilterproxy.h"

#include <datanode.h>
#include <QDebug>

PaletteFilterProxy::PaletteFilterProxy(KnowledgeService *knowledgeService,
                                       QObject *parent)
    : QSortFilterProxyModel(parent), m_knowledgeService(knowledgeService) {}

void PaletteFilterProxy::setSelectedNode(NodeView *node) {
    m_selectedNode = node;
    invalidateFilter();
}

bool PaletteFilterProxy::filterAcceptsRow(
    int source_row, const QModelIndex &source_parent) const {
    return filterAcceptsRowItself(source_row, source_parent) ||
           hasAcceptedChildren(source_row, source_parent);
}

bool PaletteFilterProxy::filterAcceptsRowItself(
    int source_row, const QModelIndex &source_parent) const {
    QModelIndex item = sourceModel()->index(source_row, 0, source_parent);
    int conceptId = item.data(Qt::UserRole).toInt();
    if (conceptId == 0) {
        return true;
    }
    return isCompatibleWithSelected(conceptId);
}

bool PaletteFilterProxy::hasAcceptedChildren(
    int source_row, const QModelIndex &source_parent) const {
    QModelIndex item = sourceModel()->index(source_row, 0, source_parent);
    if (!item.isValid()) {
        return false;
    }

    int childCount = item.model()->rowCount(item);
    if (childCount == 0) {
        return false;
    }

    for (int i = 0; i < childCount; ++i) {
        if (filterAcceptsRow(i, item)) {
            return true;
        }
    }

    return false;
}

bool PaletteFilterProxy::isCompatibleWithSelected(int conceptId) const {
    if (m_selectedNode == nullptr) {
        return true;
    }
    DataNode *selectedDataNode =
        reinterpret_cast<DataNode *>(m_selectedNode->node());
    if (selectedDataNode == nullptr || selectedDataNode->conceptId() == -1) {
        return true;
    }
    return m_knowledgeService->isCompatible(conceptId,
                                            selectedDataNode->conceptId());
}
