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

double Mesoscopic::getValueByMode(int mode) {
    switch(mode) {
        case 0: return density_;
        case 1: return flow_;
        case 2: return speed_;
    }

    return 0;
}
