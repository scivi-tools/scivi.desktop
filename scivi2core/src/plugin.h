#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include <QPointer>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include "dataflowdiagram.h"
#include "scivi2lib_global.h"

namespace scivi {

using namespace diagram;

class Plugin {
public:
    struct IntializeParams {};

    struct StartupParams {
        DataflowDiagram *diagram;
    };

    virtual ~Plugin() {}
    virtual QString name() const = 0;
    virtual void initialize(IntializeParams params) = 0;
    virtual void startup(StartupParams params) = 0;
    virtual void shutdown() {}
    virtual QPointer<QQmlComponent> createView(QQmlEngine *engine,
                                               QObject *parent = nullptr) {
        Q_ASSERT(engine);
        return nullptr;
    }
};

}  // namespace scivi

QT_BEGIN_NAMESPACE

Q_DECLARE_METATYPE(scivi::Plugin *);

#define Plugin_iid "tools.scivi.Plugin"

Q_DECLARE_INTERFACE(scivi::Plugin, Plugin_iid)

QT_END_NAMESPACE

#endif  // PLUGIN_H
