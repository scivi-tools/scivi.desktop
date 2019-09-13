#ifndef READER_H
#define READER_H

#include <QSharedPointer>

namespace ont {
    struct Ontology;
}

class Reader
{
public:
    static QSharedPointer<ont::Ontology> readFromFile(QString filename);
};

#endif // READER_H
