#ifndef RELATION_H
#define RELATION_H

#include <QString>
#include <QMap>

namespace scivi {

namespace knowledge {

class Relation
{
public:
    enum Type {
        IS_A,
        A_PART_OF,
        HAS,
        USE,
        USE_FOR,
        INSTANCE_OF,
        LANGUAGE,
        BASETYPE,
        UNKNOWN
    };

    static Type valueOf(QString value);

    static Relation *build(int id, QString name, int sourceNodeId, int destinationNodeId);

    int id() const;
    Type type() const;
    int sourceNodeId() const;
    int destinationNodeId() const;

protected:
    int m_id;
    Type m_type;
    int m_sourceNodeId;
    int m_destinationNodeId;

private:
    Relation();
};

}
}

#endif // RELATION_H
