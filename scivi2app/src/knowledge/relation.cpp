#include "relation.h"

namespace scivi {

namespace knowledge {

Relation::Relation()
{

}

Relation::Type Relation::valueOf(QString value)
{
    auto val = Relation::Type::UNKNOWN;
    if (value == "is_a")
        val = IS_A;
    else if (value == "a_part_of")
        val = A_PART_OF;
    else if (value == "use")
        val = USE;
    else if (value == "has")
        val = HAS;
    else if (value == "use_for")
        val = USE_FOR;
    else if (value == "instance_of")
        val = INSTANCE_OF;
    else if (value == "language")
        val = LANGUAGE;
    else if (value == "basetype")
        val = BASETYPE;
    return val;
}

Relation *Relation::build(int id, QString name, int sourceNodeId, int destinationNodeId)
{
    auto newRelation = new Relation();
    newRelation->m_id = id;
    newRelation->m_sourceNodeId = sourceNodeId;
    newRelation->m_destinationNodeId = destinationNodeId;
    newRelation->m_type = valueOf(name);
    return newRelation;
}

int Relation::id() const
{
    return m_id;
}

Relation::Type Relation::type() const
{
    return m_type;
}

int Relation::sourceNodeId() const
{
    return m_sourceNodeId;
}

int Relation::destinationNodeId() const
{
    return m_destinationNodeId;
}

}
}
