#include "reader.h"

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QDebug>
#include "ont/ontology.h"
#include "utils/path.h"

QSharedPointer<ont::Ontology> Reader::readFromFile(QString path)
{
    path = PathUtils::removeSchema(path);
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open ontology at " << path;
        return nullptr;
    }
    QByteArray content = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(content);
    auto ontology = QSharedPointer<ont::Ontology>::create();
    auto docObj = doc.object();
    ontology->read(docObj);
    return ontology;
}
