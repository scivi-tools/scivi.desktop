#include "knowledgeservice.h"

#include <ont/ontology.h>
#include "concept.h"
#include "triplet.h"
#include "relation.h"
#include "reader.h"
#include "ontology.h"

namespace scivi {

namespace knowledge {

KnowledgeService::KnowledgeService()
{
}

KnowledgeService::KnowledgeService(const QString &path)
{
    auto ontology = Reader::readFromFile(path);
    setOntology(*ontology);
}

KnowledgeService::KnowledgeService(ont::Ontology &ontology)
{
    setOntology(ontology);
}

void KnowledgeService::setOntology(ont::Ontology &ontology)
{
    m_ontology = fromOntolis(ontology);
}

void KnowledgeService::setOntology(QSharedPointer<Ontology> &ontology)
{
    m_ontology = ontology;
}

Concept *KnowledgeService::conceptByName(const QString &name) const
{
    for (const auto &concept: m_ontology->concepts()) {
        if (concept->name() == name)
            return concept;
    }
    return nullptr;
}

Concept *KnowledgeService::conceptById(int id) const
{
    auto it = std::find_if(m_ontology->concepts().constBegin(), m_ontology->concepts().constEnd(), [id](auto c) {
        return (c->id() == id);
    });
    return it != nullptr ? *it : nullptr;
}

QList<Concept *> KnowledgeService::flatChildrenOf(Concept *parent, DepthStrategy strategy) const
{
    Q_ASSERT(parent != nullptr);
    QList<Concept*> result;
    QList<RelationTriplet> matchedTriplets = filterTriplets(RelationTriplet(nullptr, Relation::Type::IS_A, parent));
    for (auto &triplet : matchedTriplets) {
        result << triplet.first;
        if (strategy == DepthStrategy::Recursive) {
            result << flatChildrenOf(triplet.first, strategy);
        }
    }
    return result;
}

QList<Concept *> KnowledgeService::partsOf(Concept *parent) const
{
    Q_ASSERT(parent != nullptr);
    QList<Concept*> result;
    QList<RelationTriplet> matchedTriplets = filterTriplets(RelationTriplet(nullptr, Relation::Type::A_PART_OF, parent));
    for (auto &triplet : matchedTriplets) {
        result << triplet.first;
    }
    return result;
}

QList<Concept *> KnowledgeService::subClassesOf(Concept *parent) const
{
    Q_ASSERT(parent != nullptr);
    QList<Concept*> result;
    QList<RelationTriplet> matchedTriplets = filterTriplets(RelationTriplet(parent, Relation::Type::IS_A, nullptr));
    for (auto &triplet : matchedTriplets) {
        result << triplet.third;
    }
    return result;
}

QList<Concept *> KnowledgeService::hasRelationsWithSubType(int conceptId, Concept *parentType) const
{
    QList<Concept*> result;
    auto concept = conceptById(conceptId);
    auto hasRelations = filterTriplets(RelationTriplet(concept, Relation::Type::HAS, nullptr));
    for (auto &hasRelation : hasRelations) {
        auto connectsWithParent = filterTriplets(RelationTriplet(hasRelation.third, Relation::Type::IS_A, parentType));
        if (connectsWithParent.size() > 0) {
            result << hasRelation.third;
        }
    }
    return result;
}

Concept *KnowledgeService::findMiddleConcept(Concept *from, Concept *to) const
{
    auto subClassesFrom = subClassesOf(from).toSet();
    auto childClassesTo = flatChildrenOf(to, DepthStrategy::Recursive).toSet();
    auto middleConcepts = subClassesFrom.intersect(childClassesTo);
    return *middleConcepts.constBegin();
}

QList<Concept *> KnowledgeService::instancesOf(int id, LanguageStrategy strategy) const
{
    auto concept = conceptById(id);
    QList<Concept*> result;
    auto matchedTiplets = filterTriplets(RelationTriplet(nullptr, Relation::Type::INSTANCE_OF, concept));
    auto cppNode = conceptByName("C++");
    for (const auto& triplet: matchedTiplets) {
        bool take = strategy == IGNORE;
        if (strategy == CPP) {
            auto langTriplets = filterTriplets(RelationTriplet(triplet.first, Relation::Type::LANGUAGE, cppNode));
            take = !langTriplets.isEmpty();
        }
        if (take)
            result << triplet.first;
    }
    return result;
}

bool KnowledgeService::isCompatible(int sourceConceptId, int targetConceptId) const
{
    // FIXME: Cache results
    auto typeConcept = conceptByName("Type");
    auto outputConcept = conceptByName("Output");
    auto inputConcept = conceptByName("Input");
    auto sourceOutputConcepts = hasRelationsWithSubType(sourceConceptId, outputConcept);
    auto sourceInputConcepts = hasRelationsWithSubType(sourceConceptId, inputConcept);
    auto targetOutputConcepts = hasRelationsWithSubType(targetConceptId, outputConcept);
    auto targetInputConcepts = hasRelationsWithSubType(targetConceptId, inputConcept);
    QSet<int> sourceInputTypes;
    QSet<int> sourceOutputTypes;
    QSet<int> targetInputTypes;
    QSet<int> targetOutputTypes;
    for(auto &inputConcept : sourceInputConcepts) {
        auto inputTypeConcept = findMiddleConcept(inputConcept, typeConcept);
        sourceInputTypes.insert(inputTypeConcept->id());
    }
    for(auto &outputConcept : sourceOutputConcepts) {
        auto outputTypeConcept = findMiddleConcept(outputConcept, typeConcept);
        sourceOutputTypes.insert(outputTypeConcept->id());
    }
    for (auto &inputConcept: targetInputConcepts) {
        auto inputTypeConcept = findMiddleConcept(inputConcept, typeConcept);
        targetInputTypes.insert(inputTypeConcept->id());
    }
    for(auto &outputConcept : targetOutputConcepts) {
        auto outputTypeConcept = findMiddleConcept(outputConcept, typeConcept);
        targetOutputTypes.insert(outputTypeConcept->id());
    }
    return sourceInputTypes.intersects(targetOutputTypes) || sourceOutputTypes.intersects(targetInputTypes);
}

bool isMatch(RelationTriplet triplet, RelationTriplet filterExpr)
{
    bool match = true;
    match &= filterExpr.first == nullptr || triplet.first->id() == filterExpr.first->id();
    match &= filterExpr.second == Relation::Type::UNKNOWN || triplet.second == filterExpr.second;
    match &= filterExpr.third == nullptr || triplet.third->id() == filterExpr.third->id();
    return match;
}

QList<RelationTriplet> KnowledgeService::filterTriplets(RelationTriplet filter) const
{
    QList<RelationTriplet> result;
    for (auto &triplet : m_ontology->triplets()) {
        if (isMatch(triplet, filter)) {
            result << triplet;
        }
    }
    return result;
}

Concept *findById(QList<Concept*> &concepts, int id)
{
    auto it = std::find_if(concepts.constBegin(), concepts.constEnd(), [id](auto c) {
        return c->id() == id;
    });
    return it != nullptr ? *it : nullptr;
}

QSharedPointer<Ontology> KnowledgeService::fromOntolis(ont::Ontology &ontology)
{
    QList<Concept*> concepts;
    QList<RelationTriplet> triplets;
    for (auto &node : ontology.nodes) {
        concepts.append(new Concept(node->id, node->name, node->attributes));
    }
    for (auto &relation :ontology.relations) {
        auto sourceConcept = findById(concepts, relation->sourceNodeId);
        auto destinationConcept = findById(concepts, relation->destinationNodeId);
        triplets.append(RelationTriplet(
                            sourceConcept,
                            Relation::valueOf(relation->name),
                            destinationConcept)
                        );
    }
    return QSharedPointer<Ontology>::create(concepts, triplets);
}

}
}
