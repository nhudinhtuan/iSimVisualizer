#include "busstop.h"

BusStop::BusStop(unsigned long id, unsigned long aimsunId, QPointF pos)
{
    id_ = id;
    aimsunId_ = aimsunId;
    pos_ = pos;
}
