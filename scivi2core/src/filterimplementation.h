#ifndef FILTER_IMPLEMENTATION_H
#define FILTER_IMPLEMENTATION_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

#include "scivi2lib_global.h"

namespace scivi {
namespace filters {

using FilterSettings = QMap<QString, QVariant>;

class SCIVI_LIBRARY_EXPORT FilterImplementation : public QObject {
    Q_OBJECT
public:
    explicit FilterImplementation(QObject *parent = nullptr);
    virtual ~FilterImplementation();

    virtual QString getName() const = 0;
    void setSettings(FilterSettings settings);
    QVariant &operator[](const QString &key);

signals:
    void produced(QVariant data, const QString &slotName);

public slots:
    virtual void onStart() = 0;
    virtual void consume(QVariant data, const QString &slotName) = 0;

protected:
    FilterSettings m_settings;
};

}  // namespace filters
}  // namespace scivi

#endif  // FILTER_IMPLEMENTATION_H
