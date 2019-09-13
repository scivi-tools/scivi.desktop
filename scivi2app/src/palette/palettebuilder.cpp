#include "palettebuilder.h"

namespace scivi {

namespace palette {

PaletteBuilder::PaletteBuilder(KnowledgeService *knowledgeService)
    : m_knowledgeService(knowledgeService) {}

PaletteItem *fromConcept(Concept *concept, PaletteItem *parent = nullptr) {
    return new PaletteItem(concept->id(), concept->name(), parent);
}

void PaletteBuilder::buildItem(PaletteItem *current, Concept *concept) {
    auto children = m_knowledgeService->flatChildrenOf(concept);
    for (const auto &child : children) {
        auto childPaletteItem = fromConcept(child, current);
        current->appendChild(childPaletteItem);
        buildItem(childPaletteItem, child);
    }
    auto parts = m_knowledgeService->partsOf(concept);
    for (const auto &part : parts) {
        auto partPaletteItem = fromConcept(part, current);
        current->appendChild(partPaletteItem);
        buildItem(partPaletteItem, part);
    }
}

QList<PaletteItem *> PaletteBuilder::fromConcepts(QList<Concept *> concepts) {
    QList<PaletteItem *> result;
    for (const auto &concept : concepts) {
        if (concept != nullptr) {
            auto root = fromConcept(concept);
            buildItem(root, concept);
            result << root;
        }
    }
    return result;
}

}  // namespace palette
}  // namespace scivi
