#include "typesstore.h"

#include "knowledge/concept.h"

TypesStore::TypesStore() {}

TypesStore::TypesStore(scivi::knowledge::KnowledgeService *service,
                       QQmlEngine *engine) {
    auto typeConcept = service->conceptByName("Type");
    auto typesConcepts = service->flatChildrenOf(typeConcept);
    for (auto &type : typesConcepts) {
        Type newType;
        newType.name = type->name();
        newType.color = QColor((*type)["color"].toString());
        newType.component = (*type)["layout"].toString();
        QSharedPointer<QQmlComponent> component =
            QSharedPointer<QQmlComponent>::create(engine);
        QByteArray data = newType.component.toUtf8();
        component->setData(data, QUrl("dynamicPath"));

        m_components[newType.name] = component;

        addType(newType);
    }
}

void TypesStore::addType(Type &type) { m_types.append(type); }

QSharedPointer<QQmlComponent> TypesStore::componentForType(QString typeName) {
    return m_components[typeName];
}
