#ifndef SINKFILTER_H
#define SINKFILTER_H

#include "scivi2lib_global.h"
#include <QObject>
#include <QtQuick/QQuickItem>
#include "filterimplementation.h"

namespace scivi {
namespace filters {

class SCIVI_LIBRARY_EXPORT SinkFilter: public FilterImplementation {
    Q_OBJECT
public:
    SinkFilter(QObject *parent);
    ~SinkFilter();
    void setContainer(QQuickItem* view);
protected:
    QQuickItem* m_container;
};

}
}

#endif // SINKFILTER_H
