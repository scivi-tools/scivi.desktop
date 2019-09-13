#ifndef SINKFILTER_H
#define SINKFILTER_H

#include <QObject>
#include <QtQuick/QQuickItem>
#include "filterimplementation.h"
#include "scivi2lib_global.h"

namespace scivi {
namespace filters {

class SCIVI_LIBRARY_EXPORT SinkFilter : public FilterImplementation {
    Q_OBJECT
public:
    SinkFilter(QObject *parent);
    ~SinkFilter();
    void setContainer(QQuickItem *view);

protected:
    QQuickItem *m_container;
};

}  // namespace filters
}  // namespace scivi

#endif  // SINKFILTER_H
