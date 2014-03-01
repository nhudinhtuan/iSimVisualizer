#include "trafficsignal.h"

TrafficSignal::TrafficSignal(unsigned long id, unsigned long nodeId)
{
    id_ = id;
    nodeId_ = nodeId;
}

TrafficSignal::~TrafficSignal() {
    QHash<QString, TrafficPhase*>::iterator phaseIt = phases_.begin();
    while (phaseIt != phases_.end()) {
        delete phaseIt.value();
        phaseIt++;
    }
}

void TrafficSignal::addPhase(QString name, TrafficPhase* phase) {
    phases_[name] = phase;
}

TrafficPhase* TrafficSignal::getPhase(QString name) {
    if (phases_.contains(name)) return 0;
    return phases_[name];
}
