#include "datanode.h"

#include <QDebug>
#include <QFileInfo>
#include <QObject>
#include <QPluginLoader>
#include <QPointer>
#include "filterfactory.h"

namespace scivi {
namespace diagram {

DataNode::DataNode(QObject *parent) : Node(parent) {}

DataNode::DataNode(QString name, int conceptId,
                   filters::FilterFactory *implementationFactory,
                   QList<SharedSetting> settings,
                   QList<SharedNodeSocket> inputs,
                   QList<SharedNodeSocket> outputs, QObject *parent)
    : Node(inputs, outputs, parent),
      m_conceptId(conceptId),
      m_name(name),
      m_implFactory(implementationFactory) {
    setSettings(settings);
}

QString DataNode::name() const { return m_name; }

void DataNode::setName(const QString &name) {
    this->m_name = name;
    emit nameChanged(m_name);
}

void DataNode::setSettings(QList<SharedSetting> settings) {
    m_settings = settings;
    auto model = new SettingsModel(this);
    model->setSettings(m_settings);
    m_settingsModel = QSharedPointer<SettingsModel>(model);
    emit this->settingsModelChanged();
}

QList<SharedSetting> DataNode::settings() const { return m_settings; }

void DataNode::changeSetting(int id, QVariant value) {
    for (auto &setting : m_settings) {
        if (setting->id() == id) {
            setting->setValue(value);
            break;
        }
    }
}

QObject *DataNode::settingsModel() { return m_settingsModel.data(); }

int DataNode::conceptId() { return m_conceptId; }

QJsonObject DataNode::toJsonObject() const {
    QJsonObject nodeObject;

    QJsonObject jsonNodeSettings;
    for (auto &setting : m_settings) {
        jsonNodeSettings[QString::number(setting->id())] =
            setting->value().toString();
    }
    nodeObject["id"] = QString::number(m_conceptId);
    nodeObject["settings"] = jsonNodeSettings;
    nodeObject["x"] = static_cast<double>(m_x);
    nodeObject["y"] = static_cast<double>(m_y);
    return nodeObject;
}

filters::FilterFactory *DataNode::implFactory() const { return m_implFactory; }

}  // namespace diagram
}  // namespace scivi
