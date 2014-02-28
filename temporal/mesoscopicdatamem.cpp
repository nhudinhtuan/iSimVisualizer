#include "mesoscopicdatamem.h"

MesoscopicDataMem::MesoscopicDataMem() {
}

MesoscopicDataMem::~MesoscopicDataMem() {
    // remove in-memory mesoscopic
    QHash<unsigned int, QHash<unsigned long, Mesoscopic*> >::iterator mesTickIt = mesoscopicTicks_.begin();
    while (mesTickIt != mesoscopicTicks_.end()) {
        QHash<unsigned long, Mesoscopic*>& mesoscopics = mesTickIt.value();
        for (QHash<unsigned long, Mesoscopic*>::iterator i = mesoscopics.begin(); i != mesoscopics.end(); ++i)
            delete i.value();
        mesTickIt++;
    }
    mesoscopicTicks_.clear();
}

void MesoscopicDataMem::insert(Mesoscopic* data) {
    QHash<unsigned long, Mesoscopic*>& mesoscopicList = mesoscopicTicks_[data->getTick()];
    mesoscopicList[data->getSegmentID()] = data;
}
