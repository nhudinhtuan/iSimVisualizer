#include "temporalindex.h"

TemporalIndex::TemporalIndex(QObject *parent, PreferenceManager *preferenceManager):
    QObject(parent), preferenceManager_(preferenceManager), mesoscopicData_(0), microscopicData_(0) {
    currentTick_ = 0;
}

TemporalIndex::~TemporalIndex() {
    if (mesoscopicData_) delete mesoscopicData_;
    if (microscopicData_) delete microscopicData_;
}

void TemporalIndex::setUsingMemory() {
    if (mesoscopicData_) delete mesoscopicData_;
    if (microscopicData_) delete microscopicData_;

    mesoscopicData_ = new MesoscopicDataMem();
    microscopicData_ = new MicroscopicDataMem();
}

void TemporalIndex::setUsingDB(QString dbName, iSimGUI::DataType type, bool resetDB) {

}

void TemporalIndex::reset() {
    currentTick_ = 0;
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
    if (!uniqueTicks_.isEmpty() && uniqueTicks_.last() == tick) return;
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
    if (tick < uniqueTicks_.last()) {
        currentTick_ = tick;
        return true;
    }
    return false;
}

void TemporalIndex::insertMesoscopicData(Mesoscopic* data) {
    if (!mesoscopicData_) return;
    mesoscopicData_->insert(data);
    updateUniqueTicks(data->getTick());
}

void TemporalIndex::insertAgentData(Agent *data) {
    if (!microscopicData_) return;
    microscopicData_->insertAgent(data);
    updateUniqueTicks(data->getTick());
}


AgentList* TemporalIndex::getAgent(QPoint& bottomLeft, QPoint& topRight) {
    if (!microscopicData_) return new AgentList();
    return microscopicData_->getAgent(currentTick_, bottomLeft, topRight);
}
