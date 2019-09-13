#ifndef NODE_FACTORY_H
#define NODE_FACTORY_H

#include <QColor>
#include <QList>
#include <QMap>
#include <QRgb>
#include <QSharedPointer>

#include "datanode.h"
#include "knowledge/concept.h"
#include "knowledge/knowledgeservice.h"
#include "typesstore.h"

namespace scivi {

namespace diagram {

using SharedNodeSocket = QSharedPointer<NodeSocket>;

QSharedPointer<diagram::DataNode> createNode(
    int conceptId, knowledge::KnowledgeService &service,
    TypesStore &typesStore);

}  // namespace diagram

}  // namespace scivi

#endif
