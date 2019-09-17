#include "setting.h"

namespace scivi {
namespace diagram {

Setting::Setting(const int &id, const QString &name, QVariant &value,
                 QSharedPointer<QQmlComponent> &component, QObject *parent)
    : QObject(parent) {
    m_id = id;
    m_name = name;
    m_value = value;
    m_component = component;
}

QString Setting::name() const { return m_name; }

QVariant Setting::value() const { return m_value; }

int Setting::id() const { return m_id; }

QQmlComponent *Setting::component() { return m_component.data(); }

void Setting::setValue(QVariant value) {
    m_value = value;
    emit valueChanged();
}

QMap<QString, QVariant> toMap(QList<QSharedPointer<Setting>> settings) {
    QMap<QString, QVariant> result;
    for (const auto &s : settings) {
        result[s->name()] = s->value();
    }
    return result;
}

}  // namespace diagram
}  // namespace scivi
