#ifndef PALETTEMODEL_H
#define PALETTEMODEL_H

#include <QStandardItemModel>
#include "paletteitem.h"

namespace scivi {

namespace palette {

class PaletteModel : public QStandardItemModel {
    Q_OBJECT
public:
    explicit PaletteModel(QObject *parent = nullptr);
    virtual ~PaletteModel() override = default;

    enum PaletteRoles { NameRole = Qt::DisplayRole, IdRole = Qt::UserRole };
    QHash<int, QByteArray> roleNames() const override;
    int getId(QModelIndex index);

public slots:
    void setupModel(QList<PaletteItem *> items);

private:
    QList<PaletteItem *> m_paletteItems;
    QHash<int, QByteArray> m_roleNameMapping;

    void buildTree(QStandardItem *current, QList<PaletteItem *> children);
};

}  // namespace palette
}  // namespace scivi

#endif  // PALETTEMODEL_H
