#include "ontology.h"

QList<ont::SharedNode> ont::Ontology::nodesByName(const QString &name) {
    QList<SharedNode> result;
    for(const auto &node: this->nodes) {
        if (node->name == name) {
            result.append(node);
        }
    }
    return result;
}

ont::SharedNode ont::Ontology::nodeById(int id) {
    for(const auto &node: this->nodes) {
        if (node->id == id) {
            return node;
        }
    }
    return nullptr;
}
