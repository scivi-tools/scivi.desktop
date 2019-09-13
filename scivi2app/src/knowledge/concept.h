#ifndef CONCEPT_H
#define CONCEPT_H

#include <QString>
#include <QVariantMap>

namespace scivi {

namespace knowledge {

class Concept
{
public:
    Concept(int id, QString name, QVariantMap attributes);

    int id() const;
    QString name() const;
    QVariantMap attributes();

    QVariant operator[](const QString &attibuteName);

private:
    int m_id;
    QString m_name;
    QVariantMap m_attributes;
};

}

}

#endif // CONCEPT_H
