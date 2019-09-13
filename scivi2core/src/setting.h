#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QVariant>
#include <QQmlComponent>
#include <QSharedPointer>

namespace scivi {
namespace diagram {

class Setting: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Setting)
    Q_PROPERTY(QString name READ name FINAL)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)

public:
    explicit Setting(const int &id, const QString &name, QVariant &value, QSharedPointer<QQmlComponent> &component, QObject *parent = nullptr);
    QString name() const;
    QVariant value() const;
    int id() const;
    QQmlComponent *component();
    void setValue(QVariant value);

signals:
    void valueChanged();

private:
    int m_id;
    QString m_name;
    QVariant m_value;
    QSharedPointer<QQmlComponent> m_component;
};

QMap<QString, QVariant> toMap(QList<QSharedPointer<Setting> > settings);

}
}

#endif // SETTING_H
