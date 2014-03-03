#include "temporalindex.h"

TemporalIndex::TemporalIndex(QObject *parent, PreferenceManager *preferenceManager):
    QObject(parent), preferenceManager_(preferenceManager), mesoscopicData_(0), microscopicData_(0) {
    reset();
}

TemporalIndex::~TemporalIndex() {
    reset();
}

void TemporalIndex::setUsingMemory() {
    reset();

    mesoscopicData_ = new MesoscopicDataMem();
    microscopicData_ = new MicroscopicDataMem();
}

void TemporalIndex::setUsingDB(QString dbName, bool resetDB) {

}

void TemporalIndex::reset() {
    currentTick_ = 0;
    microDataExist_ = false;
    mesoDataExist_ = false;
    uniqueTicks_.clear();
    if (mesoscopicData_) {
        mesoscopicData_ = 0;
        delete mesoscopicData_;
    }
    if (microscopicData_) {
        microscopicData_ = 0;
        delete microscopicData_;
    }
}

void TemporalIndex::updateUniqueTicks(unsigned int tick) {
    // We assume that the ticks are inserted in a chronological manner i.e. insert all with 0's then 1's, etc.
    // Thus, below checks the last element and compares it with the currently being processed tick
    if (!uniqueTicks_.isEmpty() && uniqueTicks_.last() >= tick) return;
    uniqueTicks_.append(tick);
    emit announceNewUpperTickValue(tick);
}

unsigned int TemporalIndex::jumpToNextTick() {
    if (currentTick_ < uniqueTicks_.last()) {
        currentTick_++;
        return currentTick_;
    }
    return 0;
}

bool TemporalIndex::jumpToTick(unsigned int tick) {
    if (tick < uniqueTicks_.last() && currentTick_ != tick) {
        currentTick_ = tick;
        return true;
    }
    return false;
}

void TemporalIndex::insert(Mesoscopic* data) {
    if (!mesoscopicData_) return;
    mesoscopicData_->insert(data);
    updateUniqueTicks(data->getTick());
    mesoDataExist_ = true;
}

void TemporalIndex::insert(Agent *data) {
    if (!microscopicData_) return;
    microscopicData_->insert(data);
    updateUniqueTicks(data->getTick());
    microDataExist_ = true;
}

void TemporalIndex::insert(CrossingPhaseData* crossingPhaseData) {
    if (!microscopicData_) return;
    microscopicData_->insert(crossingPhaseData);
    updateUniqueTicks(crossingPhaseData->tick);
    microDataExist_ = true;
}

void TemporalIndex::insert(TrafficPhaseData* trafficPhaseData) {
    if (!microscopicData_) return;
    microscopicData_->insert(trafficPhaseData);
    updateUniqueTicks(trafficPhaseData->tick);
    microDataExist_ = true;
}

AgentList* TemporalIndex::getAgent(QPoint& bottomLeft, QPoint& topRight) {
    if (!microscopicData_ || !microDataExist_) return new AgentList();
    return microscopicData_->getAgent(currentTick_, bottomLeft, topRight);
}

void TemporalIndex::updateCrossingPhaseData(QPoint& bottomLeft, QPoint& topRight) {
    if (!microscopicData_ || !microDataExist_) return;
    microscopicData_->updateCrossingPhaseData(currentTick_, bottomLeft, topRight);
}

int TemporalIndex::getCrossingPhaseColor(unsigned long crossingId) {
    if (!microscopicData_ || !microDataExist_) return -1;
    return microscopicData_->getCrossingPhaseColor(currentTick_, crossingId);
}

TrafficPhaseData* TemporalIndex::getTrafficPhaseData(unsigned long id) {
    if (!microscopicData_ || !microDataExist_) return 0;
    return microscopicData_->getTrafficPhaseData(currentTick_, id);
}
