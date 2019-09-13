#include "concept.h"

namespace scivi {

namespace knowledge {

Concept::Concept(int id, QString name, QVariantMap attributes)
    : m_id(id), m_name(name), m_attributes(attributes) {}

int Concept::id() const { return m_id; }

QString Concept::name() const { return m_name; }

QVariantMap Concept::attributes() { return m_attributes; }

QVariant Concept::operator[](const QString &attibuteName) {
    return m_attributes.contains(attibuteName)
               ? m_attributes.value(attibuteName)
               : QVariant();
}

}  // namespace knowledge
}  // namespace scivi
