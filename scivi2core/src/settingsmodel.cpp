#include "settingsmodel.h"

namespace scivi {

SettingsModel::SettingsModel(QObject* parent):
    QAbstractListModel(parent)
{
}

void SettingsModel::setSettings(QList<QSharedPointer<Setting>> &settings)
{
    beginResetModel();
    m_settings = settings;
    endResetModel();
}

int SettingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_settings.size();
}

QVariant SettingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    auto setting = m_settings[index.row()];
    if (role == NameRole)
        return setting->name();
    else if (role == ValueRole)
        return setting->value();
    else if (role == ComponentRole) {
        QVariant value;
        value.setValue(setting->component());
        return value;
    }
    return QVariant();
}

QHash<int, QByteArray> SettingsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ComponentRole] = "fieldComponent";
    roles[ValueRole] = "value";
    return roles;
}

bool SettingsModel::setData(int row, int column, const QVariant &data)
{
    if (column == 1) {
        m_settings[row]->setValue(data);
        emit dataChanged(index(row), index(row));
        return true;
    }
    return true;
}

bool SettingsModel::saveValue(QString name, const QVariant &data)
{
    int row = 0;
    for (int i = 0; i < m_settings.size(); i++) {
        if (m_settings[i]->name() == name) {
            row = i;
            break;
        }
    }
    auto setting = m_settings[row];
    setting->setValue(data);
    emit dataChanged(index(row), index(row));
    return false;
}



}
