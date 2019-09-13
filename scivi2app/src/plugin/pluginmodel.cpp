#include "pluginmodel.h"

#include "plugin.h"

namespace scivi {

PluginModel::PluginModel(QObject *parent) : QAbstractListModel(parent) {}

PluginModel::~PluginModel() {
    for (const auto &plugin : m_plugins) {
        plugin->shutdown();
    }
}

void PluginModel::setPluginList(QList<Plugin *> plugins) {
    beginInsertRows(QModelIndex(), 0, plugins.size());
    m_plugins = plugins;
    endInsertRows();
}

QList<Plugin *> PluginModel::plugins() const { return m_plugins; }

void PluginModel::addPlugin(Plugin *plugin) {
    beginInsertRows(QModelIndex(), m_plugins.size() - 1, m_plugins.size());
    m_plugins.append(plugin);
    endInsertRows();
}

Plugin *PluginModel::findByName(QString name) {
    const auto it =
        std::find_if(m_plugins.cbegin(), m_plugins.cend(),
                     [name](Plugin *plugin) { return plugin->name() == name; });
    return it != m_plugins.cend() ? *it : nullptr;
}

int PluginModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_plugins.size();
}

QVariant PluginModel::data(const QModelIndex &index, int role) const {
    const int row = index.row();
    if (row < 0 || row >= m_plugins.size()) {
        return QVariant();
    }
    const auto plugin = m_plugins.at(row);
    if (role == Qt::DisplayRole) {
        return plugin->name();
    } else if (role == Qt::UserRole) {
        return qVariantFromValue(plugin);
    }
    return QVariant();
}

}  // namespace scivi
