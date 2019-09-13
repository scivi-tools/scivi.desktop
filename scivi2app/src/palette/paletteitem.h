#ifndef PALETTEITEM_H
#define PALETTEITEM_H

#include <QString>
#include <QList>

namespace scivi {

namespace palette {

class PaletteItem
{
public:
    explicit PaletteItem(int id, QString name, PaletteItem *parent = nullptr);
    ~PaletteItem();

    QString name() const;
    int id() const;

    void appendChild(PaletteItem *child);

    PaletteItem *child(int row);
    QList<PaletteItem*> children();
    int childCount() const;
    int row() const;
    PaletteItem *parentItem();

private:
    int m_itemId;
    QString m_itemName;
    QList<PaletteItem*> m_childItems;
    PaletteItem *m_parentItem;
};

}
}

#endif // PALETTEITEM_H
