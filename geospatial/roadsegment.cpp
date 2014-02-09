#include "roadsegment.h"

RoadSegment::RoadSegment(unsigned long id, unsigned long aimsunId, unsigned long fromNodeId,
                         unsigned long toNodeId, int maxSpeed, int width, int nLane) {
    id_ = id;
    aimsunId_ = aimsunId;
    fromNodeId_ = fromNodeId;
    toNodeId_ = toNodeId;
    maxSpeed_ = maxSpeed;
    width_ = width;
    nLane_ = nLane;
}
