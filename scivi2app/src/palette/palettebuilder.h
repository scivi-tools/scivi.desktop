#ifndef PALETTEBUILDER_H
#define PALETTEBUILDER_H

#include <QList>
#include "knowledge/concept.h"
#include "knowledge/knowledgeservice.h"
#include "paletteitem.h"

using namespace scivi::knowledge;

namespace scivi {

namespace palette {

class PaletteBuilder {
public:
    PaletteBuilder(KnowledgeService *knowledgeService);

    QList<PaletteItem *> fromConcepts(QList<Concept *> concepts);

private:
    KnowledgeService *m_knowledgeService;

    void buildItem(PaletteItem *current, Concept *concept);
};

}  // namespace palette
}  // namespace scivi

#endif  // PALETTEBUILDER_H
