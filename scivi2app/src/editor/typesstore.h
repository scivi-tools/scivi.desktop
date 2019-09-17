#ifndef TYPESSTORE_H
#define TYPESSTORE_H

#include <QColor>
#include <QList>
#include <QQmlComponent>
#include <QString>

#include "knowledge/knowledgeservice.h"

struct Type {
    QString name;
    QColor color;
    QString component;
};

class TypesStore {
public:
    TypesStore();
    TypesStore(scivi::knowledge::KnowledgeService *service, QQmlEngine *engine);
    void addType(Type &type);
    QSharedPointer<QQmlComponent> componentForType(QString typeName);

private:
    QList<Type> m_types;
    QMap<QString, QSharedPointer<QQmlComponent>> m_components;
};

#endif  // TYPESSTORE_H
