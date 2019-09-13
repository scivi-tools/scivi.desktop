#include "palettemodel.h"

namespace scivi {
namespace palette {

PaletteModel::PaletteModel(QObject *parent) : QStandardItemModel(parent)
{
    m_roleNameMapping[NameRole] = "name_role";
    m_roleNameMapping[IdRole] = "id_role";
}

QHash<int, QByteArray> PaletteModel::roleNames() const
{
    return m_roleNameMapping;
}

int PaletteModel::getId(QModelIndex index)
{
    auto idRole = index.data(IdRole);
    return idRole.toInt();
}

void PaletteModel::buildTree(QStandardItem *current, QList<PaletteItem*> children)
{
    if (children.isEmpty()) return;
    foreach (auto child, children) {
        auto childItem = new QStandardItem(child->name());
        childItem->setData(static_cast<int>(child->id()), IdRole);
        current->appendRow(childItem);
        buildTree(childItem, child->children());
    }
}

void PaletteModel::setupModel(QList<PaletteItem *> items)
{
    clear();
    foreach(auto item, items) {
        auto parentItem = new QStandardItem(item->name());
        this->appendRow(parentItem);
        buildTree(parentItem, item->children());
    }
    m_paletteItems = items;
}


}
}
