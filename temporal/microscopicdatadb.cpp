#include "microscopicdatadb.h"

MicroscopicDataDB::MicroscopicDataDB() {

}

MicroscopicDataDB::~MicroscopicDataDB() {

}

void MicroscopicDataDB::insert(CrossingPhaseData *crossingPhaseData) {

}

void MicroscopicDataDB::insert(TrafficPhaseData* trafficPhaseData) {

}

void MicroscopicDataDB::insert(Agent* data) {

}

void MicroscopicDataDB::updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {

}

AgentList* MicroscopicDataDB::getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    return new AgentList();
}

int MicroscopicDataDB::getCrossingPhaseColor(unsigned int tick, unsigned long crossingId) {
    return 0;
}

TrafficPhaseData* MicroscopicDataDB::getTrafficPhaseData(unsigned int tick, unsigned long id) {
    return 0;
}
