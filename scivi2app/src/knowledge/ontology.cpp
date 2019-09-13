#include "ontology.h"

#include "knowledge/concept.h"
#include "knowledge/relation.h"

namespace scivi {

namespace knowledge {

Ontology::Ontology(QList<Concept *> concepts, QList<RelationTriplet> triplets)
    : m_concepts(concepts), m_triplets(triplets)
{

}

QList<Concept *> Ontology::concepts()
{
    return m_concepts;
}

QList<RelationTriplet> Ontology::triplets()
{
    return m_triplets;
}

}
}
