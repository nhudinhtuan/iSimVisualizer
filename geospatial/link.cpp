#include "link.h"

Link::Link(unsigned long id, QString name, unsigned long startNodeId, unsigned long endNodeId)
{
    name_ = name;
    id_ = id;
    startNodeId_ = startNodeId;
    endNodeId_ = endNodeId;
}
