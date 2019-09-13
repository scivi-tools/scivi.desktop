#include "filterimplementation.h"

namespace scivi {
namespace filters {

scivi::filters::FilterImplementation::FilterImplementation(QObject *parent)
    : QObject(parent) {}

FilterImplementation::~FilterImplementation() {}

void FilterImplementation::setSettings(
    scivi::filters::FilterSettings settings) {
    m_settings = settings;
}

QVariant &FilterImplementation::operator[](const QString &key) {
    return m_settings[key];
}

}  // namespace filters
}  // namespace scivi
