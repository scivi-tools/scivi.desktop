#ifndef DATANODE_H
#define DATANODE_H

#include <QSharedPointer>
#include <QString>
#include <QObject>
#include <QVariantMap>
#include <QAbstractListModel>
#include "filterfactory.h"
#include "node.h"
#include "setting.h"
#include "settingsmodel.h"

namespace scivi {
namespace diagram {

using SharedSetting = QSharedPointer<Setting>;

class DataNode : public Node
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QObject* settingsModel READ settingsModel NOTIFY settingsModelChanged)
public:
    explicit DataNode(QObject *parent = nullptr);
    explicit DataNode(QString name, int conceptId = -1, scivi::filters::FilterFactory* implementationFactory = nullptr,
                      QList<SharedSetting> settings = {}, QList<SharedNodeSocket> inputs = {}, QList<SharedNodeSocket> outputs = {},
                      QObject *parent = nullptr);
    QString name() const;
    void setName(const QString &name);
    void setSettings(QList<SharedSetting> settings);
    virtual QList<SharedSetting> settings() const;
    void changeSetting(int id, QVariant value);
    QObject* settingsModel();
    int conceptId();

    QJsonObject toJsonObject() const override;

    scivi::filters::FilterFactory* implFactory() const;

signals:
    void nameChanged(QString);
    void settingsModelChanged();

protected:
    int m_conceptId {-1};
    QString m_name;
    QList<SharedSetting> m_settings;
    QSharedPointer<SettingsModel> m_settingsModel;
    scivi::filters::FilterFactory* m_implFactory = { nullptr };
};

}
}

QML_DECLARE_TYPE(scivi::diagram::DataNode)

#endif // DATANODE_H
