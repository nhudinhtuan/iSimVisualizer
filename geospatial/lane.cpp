#include "lane.h"

Lane::Lane(unsigned long segmentId, unsigned int index, bool isSideWalk)
{
    segmentId_ = segmentId;
    index_ = index;
    isSideWalk_ = isSideWalk;
}

QPointF Lane::getFirstPoint() {
    if (polyline_.size() == 0) return QPointF(0, 0);
    return polyline_[0];
}

QPointF Lane::getLastPoint() {
    if (polyline_.size() == 0) return QPointF(0, 0);
    return polyline_[polyline_.size()-1];
}
