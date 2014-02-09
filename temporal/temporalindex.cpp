#include "temporalindex.h"

TemporalIndex::TemporalIndex(QObject *parent, PreferenceManager *preferenceManager):
    QObject(parent), db_(0), preferenceManager_(preferenceManager) {
}

TemporalIndex::~TemporalIndex() {

}

void TemporalIndex::setUsingMemory() {
    if (db_) {
        delete db_;
        db_ = 0;
    }
}

void TemporalIndex::setUsingDB(QString dbName, iSimGUI::DataType type, bool resetDB) {
    if (db_) delete db_;
    db_ = new DBio(dbName, type);
    if (resetDB) {

    }
}

void TemporalIndex::releaseMemory() {
    // remove in-memory mesoscopic
}

void TemporalIndex::updateUniqueTicks(unsigned int tick) {
    // We assume that the ticks are inserted in a chronological manner i.e. insert all with 0's then 1's, etc.
    // Thus, below checks the last element and compares it with the currently being processed tick
    if (!uniqueTicks_.isEmpty() && uniqueTicks_.last() == tick) return;
    uniqueTicks_.append(tick);
    if (db_ == 0) {
        emit announceUpperBoundTickValueChange(tick);
    } else {
        //
    }
}

void TemporalIndex::insertMesoscopicData(Mesoscopic& mesoscopicData) {
    unsigned int tick = mesoscopicData.getTick();
    if (!db_) {

    } else {
        mesoscopicMutex_.lock();
        Mesoscopic* inMemMesoscopicData = new Mesoscopic(mesoscopicData);
        // We insert the mesoscopic data to a new list if it's the first data for a particular segment
        // We always append to the end of the list afterwards as we assume that the mesoscopic data series
        // has integrity.
        // ASSUMPTIONS: tick starts with 0; tick series follows a stepwise integer fashion which grows to an upper bound;
        //              all segments have corresponding mesoscopic data for all ticks present in the global data set
        QHash<const unsigned long, QList<Mesoscopic*> *>::const_iterator mesoscopicDataListFinder = mesoscopicDataIndexedBySegmentID_.find(mesoscopicData.getSegmentID());
        if (mesoscopicDataListFinder != mesoscopicDataIndexedBySegmentID_.end()) {
            QList<Mesoscopic*> *mesoscopicDataList = mesoscopicDataListFinder.value();
            mesoscopicDataList->push_back(inMemMesoscopicData);
        }
        else {
            QList<Mesoscopic*> *mesoscopicDataList = new QList<Mesoscopic*>();
            mesoscopicDataList->push_back(inMemMesoscopicData);
            mesoscopicDataIndexedBySegmentID_.insert(inMemMesoscopicData->getSegmentID(), mesoscopicDataList);
        }
        mesoscopicMutex_.unlock();
    }
    updateUniqueTicks(tick);
}
