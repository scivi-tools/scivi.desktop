#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include <QAbstractListModel>
#include <QObject>

namespace scivi {

class Plugin;

class PluginModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit PluginModel(QObject *parent = nullptr);
    ~PluginModel() override;

    void setPluginList(QList<Plugin *> plugins);
    QList<Plugin *> plugins() const;
    void addPlugin(Plugin *plugin);
    Plugin *findByName(QString name);

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QList<Plugin *> m_plugins;
};

}  // namespace scivi

#endif  // PLUGINMODEL_H
