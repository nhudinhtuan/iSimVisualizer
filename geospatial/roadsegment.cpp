#include "roadsegment.h"

RoadSegment::RoadSegment(unsigned long linkId, unsigned long id, unsigned long aimsunId, unsigned long fromNodeId,
                         unsigned long toNodeId, int maxSpeed, int width, int nLane) {
    linkId_ = linkId;
    id_ = id;
    aimsunId_ = aimsunId;
    fromNodeId_ = fromNodeId;
    toNodeId_ = toNodeId;
    maxSpeed_ = maxSpeed;
    width_ = width;
    nLane_ = nLane;
}

RoadSegment::~RoadSegment() {
    for (int i = 0; i < lanes_.size(); ++i) {
        delete lanes_[i];
    }
}

Lane* RoadSegment::getLane(int index) {
    if (index < 0 || lanes_.size() == 0 || index + 1 > lanes_.size())
        return 0;
    return lanes_[index];
}

Lane* RoadSegment::getMiddleLane() {
    if (lanes_.size() == 0) return 0;
    return lanes_[lanes_.size()/2];
}
