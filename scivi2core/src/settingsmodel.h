#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QPointer>
#include <QSharedPointer>
#include <QString>

#include "./setting.h"

namespace scivi {

using namespace diagram;

class SettingsModel : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsModel)
public:
    enum SettingsRoles {
        NameRole = Qt::UserRole + 1,
        ComponentRole,
        ValueRole
    };

    SettingsModel(QObject *parent = nullptr);
    void setSettings(QList<QSharedPointer<Setting>> &settings);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE bool setData(int row, int column, const QVariant &data);
    Q_INVOKABLE bool saveValue(QString name, const QVariant &data);

private:
    QList<QSharedPointer<Setting>> m_settings;
};

}  // namespace scivi
#endif  // SETTINGSMODEL_H
