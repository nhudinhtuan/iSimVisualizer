#include "mesoscopic.h"

Mesoscopic::Mesoscopic(Mesoscopic& another) {
    segmentID_ = another.getSegmentID();
    tick_ = another.getTick();
    speed_ = another.getSpeed();
    flow_ = another.getFlow();
    density_ = another.getDensity();
}

void Mesoscopic::update(Mesoscopic& another) {
    segmentID_ = another.getSegmentID();
    tick_ = another.getTick();
    speed_ = another.getSpeed();
    flow_ = another.getFlow();
    density_ = another.getDensity();
}
