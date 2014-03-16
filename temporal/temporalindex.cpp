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

void TemporalIndex::setUsingDB(int fileId) {
    reset();
    mesoscopicData_ = new MesoscopicDataMem();
    microscopicData_ = new MicroscopicDataDB(fileId);
}

void TemporalIndex::reset() {
    currentTick_ = 0;
    microDataExist_ = false;
    mesoDataExist_ = false;
    maxTick_ = 0;
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
    if (maxTick_ >= tick) return;
    maxTick_ = tick;
    emit announceNewUpperTickValue(tick);
}

unsigned int TemporalIndex::jumpToNextTick() {
    if (currentTick_ < maxTick_) {
        currentTick_++;
        return currentTick_;
    }
    return 0;
}

bool TemporalIndex::jumpToTick(unsigned int tick) {
    if (tick <= maxTick_ && currentTick_ != tick) {
        currentTick_ = tick;
        return true;
    }
    return false;
}

void TemporalIndex::insert(Mesoscopic* data) {
    if (!mesoscopicData_) return;
    mesoscopicData_->insert(data);
    updateUniqueTicks(data->getTick());
    if (!mesoDataExist_) {
        emit announceMesoscopicDataExist();
        mesoDataExist_ = true;
    }
}

void TemporalIndex::insert(Agent& data) {
    if (!microscopicData_) return;
    microscopicData_->insert(data);
    updateUniqueTicks(data.getTick());
    if (!microDataExist_) {
        emit announceMicroDataExist();
        microDataExist_ = true;
    }
}

void TemporalIndex::insert(CrossingPhaseData& crossingPhaseData) {
    if (!microscopicData_) return;
    microscopicData_->insert(crossingPhaseData);
    updateUniqueTicks(crossingPhaseData.tick);
    microDataExist_ = true;
}

void TemporalIndex::insert(TrafficPhaseData& trafficPhaseData) {
    if (!microscopicData_) return;
    microscopicData_->insert(trafficPhaseData);
    updateUniqueTicks(trafficPhaseData.tick);
    microDataExist_ = true;
}

AgentList* TemporalIndex::getAgent(QPoint& bottomLeft, QPoint& topRight) {
    if (!microscopicData_ || !microDataExist_) return new AgentList();
    return microscopicData_->getAgent(currentTick_, bottomLeft, topRight);
}

void TemporalIndex::updateCrossingPhaseData(QPoint& bottomLeft, QPoint& topRight) {
    if (!microscopicData_ || !microDataExist_) return;
    microscopicData_->updatePhaseData(currentTick_, bottomLeft, topRight);
}

int TemporalIndex::getCrossingPhaseColor(unsigned long crossingId) {
    if (!microscopicData_ || !microDataExist_) return -1;
    return microscopicData_->getCrossingPhaseColor(currentTick_, crossingId);
}

TrafficPhaseData TemporalIndex::getTrafficPhaseData(unsigned long id) {
    if (!microscopicData_ || !microDataExist_) return TrafficPhaseData();
    return microscopicData_->getTrafficPhaseData(currentTick_, id);
}

Mesoscopic* TemporalIndex::getMesoscopic(unsigned long segmentId) {
    if (!mesoscopicData_ || !mesoDataExist_) return 0;
    return mesoscopicData_->getMesoscopic(currentTick_, segmentId);
}

void TemporalIndex::finishInsertingData() {
    if (!microscopicData_ || !microDataExist_) return;
    microscopicData_->finishInsertingData();
}
