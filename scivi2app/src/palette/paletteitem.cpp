#include "paletteitem.h"

namespace scivi {
namespace palette {

PaletteItem::PaletteItem(int id, QString name, PaletteItem *parent)
{
    m_parentItem = parent;
    m_itemId = id;
    m_itemName = name;
}

PaletteItem::~PaletteItem()
{
    for (auto c: m_childItems) {
        delete c;
    }
}

QString PaletteItem::name() const
{
    return m_itemName;
}

int PaletteItem::id() const
{
    return m_itemId;
}

void PaletteItem::appendChild(PaletteItem *child)
{
    m_childItems.append(child);
}

PaletteItem *PaletteItem::child(int row)
{
    return m_childItems.value(row);
}

QList<PaletteItem *> PaletteItem::children()
{
    return m_childItems;
}

int PaletteItem::childCount() const
{
    return m_childItems.size();
}

int PaletteItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<PaletteItem*>(this));
    return 0;
}

PaletteItem *PaletteItem::parentItem()
{
    return m_parentItem;
}

}
}
