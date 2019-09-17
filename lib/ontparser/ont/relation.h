#ifndef ONT_RELATION_H
#define ONT_RELATION_H

#include <QString>
#include <QJsonObject>
#include <QVariantMap>

#include "ontologydata.h"

namespace ont {

struct OntologyData;

struct Relation : public OntologyData {
    int sourceNodeId;
    int destinationNodeId;

    void read(const QJsonObject &json) override {
        OntologyData::read(json);
        this->sourceNodeId = json["source_node_id"].toString().toInt();
        this->destinationNodeId = json["destination_node_id"].toString().toInt();
    }
};

}

#endif // ONT_RELATION_H
