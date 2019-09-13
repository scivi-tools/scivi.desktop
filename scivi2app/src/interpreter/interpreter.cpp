#include "interpreter.h"

#include <QPluginLoader>
#include <QFileInfo>
#include <sinkfilter.h>

#include <node.h>
#include <datanode.h>
#include <edge.h>
#include <dataflowdiagram.h>

namespace scivi {
using namespace filters;
namespace diagram {

Interpreter::Interpreter(QObject *parent): QObject(parent)
{

}

Interpreter::~Interpreter()
{
    qDeleteAll(m_implementations.begin(), m_implementations.end());
    m_implementations.clear();
}

void Interpreter::interpret(DataflowDiagram *diagram, QQuickItem *container)
{
    auto nodes = diagram->nodes();
    for (const auto& node: nodes) {
        DataNode *dataNode = qobject_cast<DataNode*>(node.data());
        if (dataNode != nullptr) {
            auto factory = dataNode->implFactory();
            if (factory == nullptr) {
                qDebug() << "Filter factory for node " << dataNode->name() << " is null";
                continue;
            }
            auto impl = factory->create(this);
            impl->setSettings(toMap(dataNode->settings()));
            QObject::connect(this, &Interpreter::interpretationStarted, impl, &FilterImplementation::onStart);
            m_implementations.insert(node->id(), impl);
        }
    }
    auto edges = diagram->edges();
    for (const auto& edge: edges) {
        auto src = edge->src();
        auto dest = edge->dest();
        auto srcName = src->label();
        auto destName = dest->label();
        auto srcImpl = m_implementations[src->node()->id()];
        auto destImpl = m_implementations[dest->node()->id()];
        QObject::connect(srcImpl, &FilterImplementation::produced, destImpl, [srcName, destName, destImpl](QVariant data, const QString &socketName) {
            if (socketName == srcName) {
                destImpl->consume(data, destName);
            }
        });
    }
    bool hasSinkFilter = false;
    for (auto f: m_implementations.values()) {
        auto sinkFilter = dynamic_cast<SinkFilter*>(f);
        if (sinkFilter != nullptr) {
            sinkFilter->setContainer(container);
            hasSinkFilter = true;
        }
    }
    if (!hasSinkFilter) {
        qWarning() << "Cannot find sink filter for visualization";
    }
    emit interpretationStarted();
}

}
}
