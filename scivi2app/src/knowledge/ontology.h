#ifndef ONTOLOGY_H
#define ONTOLOGY_H

#include <QList>
#include "knowledge/triplet.h"

namespace scivi {

namespace knowledge {

class Concept;

class Ontology {
public:
    Ontology(QList<Concept*> concepts, QList<RelationTriplet> triplets);
    QList<Concept*> concepts();
    QList<RelationTriplet> triplets();

private:
    QList<Concept*> m_concepts;
    QList<RelationTriplet> m_triplets;
};

}  // namespace knowledge

}  // namespace scivi

#endif  // ONTOLOGY_H
