#ifndef ONT_ONTOLOGYDATA_H
#define ONT_ONTOLOGYDATA_H

#include <QString>
#include <QVariantMap>
#include <QJsonObject>

namespace ont {

struct OntologyData
{
    int id;
    QString name;
    QVariantMap attributes;
    virtual ~OntologyData() = default;

    virtual void read(const QJsonObject &json) {
        this->id = json["id"].toString().toInt();
        this->name = json["name"].toString();
        this->attributes = json["attributes"].toObject().toVariantMap();
    }
};

}

#endif // ONT_ONTOLOGYDATA_H
