#include "laneconnector.h"

LaneConnector::LaneConnector(unsigned long id, unsigned long fromSegment, unsigned int fromLane, unsigned long toSegment, unsigned int toLane)
{
    id_ = id;
    fromSegment_ = fromSegment;
    fromLane_ = fromLane;
    toSegment_ = toSegment;
    toLane_ = toLane;
}

void LaneConnector::setPoints(QPointF fromPoint, QPointF toPoint) {
    fromPoint_ = fromPoint;
    toPoint_ = toPoint;
}
