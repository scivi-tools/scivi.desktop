#ifndef KNOWLEDGESERVICE_H
#define KNOWLEDGESERVICE_H

#include <QList>
#include <QSharedPointer>
#include "triplet.h"

namespace ont {
    struct Concept;
    struct Ontology;
    struct Relation;
}

namespace scivi {

namespace knowledge {

class Ontology;
class Concept;

class KnowledgeService
{
public:
    KnowledgeService();
    KnowledgeService(const QString &path);
    KnowledgeService(ont::Ontology &ontology);

    enum DepthStrategy {
        Direct,
        Recursive
    };

    enum LanguageStrategy {
        CPP,
        IGNORE
    };

    void setOntology(ont::Ontology &ontology);
    void setOntology(QSharedPointer<Ontology> &ontology);
    Concept *conceptByName(const QString &name) const;
    Concept *conceptById(int id) const;
    QList<RelationTriplet> filterTriplets(RelationTriplet filter) const;
    QList<Concept*> flatChildrenOf(Concept *parent, DepthStrategy strategy = Direct) const;
    QList<Concept*> partsOf(Concept *parent) const;
    QList<Concept*> subClassesOf(Concept *parent) const;
    QList<Concept*> hasRelationsWithSubType(int conceptId, Concept *parentType) const;
    Concept* findMiddleConcept(Concept *from, Concept *to) const;
    QList<Concept*> instancesOf(int id, LanguageStrategy strategy = CPP) const;
    bool isCompatible(int sourceConceptId, int targetConceptId) const;

private:
    QSharedPointer<Ontology> m_ontology;

    QSharedPointer<Ontology> fromOntolis(ont::Ontology &ontology);
};

}

}


#endif // KNOWLEDGESERVICE_H
