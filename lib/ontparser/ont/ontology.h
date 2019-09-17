#ifndef ONT_ONTOLOGY_H
#define ONT_ONTOLOGY_H

#include <QJsonObject>
#include <QJsonArray>
#include <QSharedPointer>
#include <QWeakPointer>
#include "node.h"
#include "relation.h"

namespace ont {

using SharedNode = QSharedPointer<Node>;
using WeakNode = QSharedPointer<Node>;
using SharedRelation = QSharedPointer<Relation>;
using WeakRelation = QWeakPointer<Relation>;

struct Ontology {
    QList<SharedNode> nodes;
    QList<SharedRelation> relations;
    QMap<int, QList<WeakRelation>> nodeRelations; // (Source node) * (Relation)
    QMap<int, QList<WeakRelation>> reversedRelations; // (Destination node) * (Relation)

    void read(const QJsonObject &json) {
        auto nodesArray = json["nodes"].toArray();
        for(const auto node: nodesArray) {
            auto nodeObject = node.toObject();
            SharedNode newNode = SharedNode::create();
            newNode->read(nodeObject);
            nodes.append(newNode);
        }
        auto relationsArray = json["relations"].toArray();
        for(const auto relation: relationsArray) {
            auto relationObject = relation.toObject();
            SharedRelation newRelation = SharedRelation::create();
            newRelation->read(relationObject);
            relations.append(newRelation);
            nodeRelations[newRelation->sourceNodeId].append(newRelation);
            reversedRelations[newRelation->destinationNodeId].append(newRelation);
        }
    }
    QList<SharedNode> nodesByName(const QString &name);
    SharedNode nodeById(int id);
};

}

#endif // ONT_ONTOLOGY_H
