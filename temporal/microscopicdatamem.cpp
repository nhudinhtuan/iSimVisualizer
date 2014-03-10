#include "microscopicdatamem.h"
#define CROSSING_SEARCH_GAP 20

MicroscopicDataMem::MicroscopicDataMem()
{
}

MicroscopicDataMem::~MicroscopicDataMem() {
    QHash<unsigned int, AgentTree*>::iterator agentTreeIt = agentTicks_.begin();
    while (agentTreeIt != agentTicks_.end()) {
        delete agentTreeIt.value();
        agentTreeIt++;
    }

    // remove in-memory mesoscopic
    QHash<unsigned int, QHash<unsigned long, CrossingPhaseData*> >::iterator crossingPhaseDataTickIt = crossingPhaseData_.begin();
    while (crossingPhaseDataTickIt != crossingPhaseData_.end()) {
        QHash<unsigned long, CrossingPhaseData*>& crossingPhaseData = crossingPhaseDataTickIt.value();
        for (QHash<unsigned long, CrossingPhaseData*>::iterator i = crossingPhaseData.begin(); i != crossingPhaseData.end(); ++i)
            delete i.value();
        crossingPhaseDataTickIt++;
    }

    // remove in-memory mesoscopic
    QHash<unsigned int, QHash<unsigned long, TrafficPhaseData*> >::iterator trafficPhaseDataTickIt = trafficPhaseData_.begin();
    while (trafficPhaseDataTickIt != trafficPhaseData_.end()) {
        QHash<unsigned long, TrafficPhaseData*>& trafficPhaseData = trafficPhaseDataTickIt.value();
        for (QHash<unsigned long, TrafficPhaseData*>::iterator i = trafficPhaseData.begin(); i != trafficPhaseData.end(); ++i)
            delete i.value();
        trafficPhaseDataTickIt++;
    }
}

void MicroscopicDataMem::insert(CrossingPhaseData *crossingPhaseData) {
    QHash<unsigned long, CrossingPhaseData*>& crossingPhase = crossingPhaseData_[crossingPhaseData->tick];
    crossingPhase[crossingPhaseData->crossingId] = crossingPhaseData;
}

void MicroscopicDataMem::insert(TrafficPhaseData* trafficPhaseData) {
    QHash<unsigned long, TrafficPhaseData*>& trafficPhase = trafficPhaseData_[trafficPhaseData->tick];
    trafficPhase[trafficPhaseData->id] = trafficPhaseData;
}

void MicroscopicDataMem::insert(Agent& data) {
    Agent *agent = 0;
    switch (data.getType()) {
        case iSimGUI::AGENT_DRIVER:
            agent = new Driver(dynamic_cast<Driver&>(data));
            break;
        case iSimGUI::AGENT_BUS:
            agent = new BusDriver(dynamic_cast<BusDriver&>(data));
            break;
        case iSimGUI::AGENT_PEDESTRIAN:
            agent = new Pedestrian(dynamic_cast<Pedestrian&>(data));
            break;
        default: break;
    }
    unsigned int tick = agent->getTick();
    if (agentTicks_.contains(tick)) {
        AgentTree *tree = agentTicks_[tick];
        tree->insert(agent);
    } else {
        AgentTree *tree = new AgentTree();
        tree->insert(agent);
        agentTicks_[tick] = tree;
    }
}

void MicroscopicDataMem::updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    // already in memory
    // no need to update
    Q_UNUSED(tick)
    Q_UNUSED(bottomLeft)
    Q_UNUSED(topRight)
}

AgentList* MicroscopicDataMem::getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    if (agentTicks_.contains(tick)) {
        return new AgentList(agentTicks_[tick]->query(bottomLeft, topRight));
    } else {
        return new AgentList();
    }
}

int MicroscopicDataMem::getCrossingPhaseColor(unsigned int tick, unsigned long crossingId) {
    if (crossingPhaseData_.size() == 0)
        return -1;
    long nearesTick_ = tick;
    while (nearesTick_>= 0 && tick - nearesTick_ <=  CROSSING_SEARCH_GAP) {
        if (crossingPhaseData_.contains(nearesTick_)) {
            if (crossingPhaseData_[nearesTick_].contains(crossingId)) {
                return crossingPhaseData_[nearesTick_][crossingId]->color;
            }
            return -1;
        }
        nearesTick_--;
    }
    return -1;
}

TrafficPhaseData* MicroscopicDataMem::getTrafficPhaseData(unsigned int tick, unsigned long id) {
    if (trafficPhaseData_.size() == 0) return 0;
    long nearesTick_ = tick;
    while (nearesTick_>= 0 && tick - nearesTick_ <=  CROSSING_SEARCH_GAP) {
        if (trafficPhaseData_.contains(nearesTick_)) {
            if (trafficPhaseData_[nearesTick_].contains(id)) {
                return trafficPhaseData_[nearesTick_][id];
            }
            return 0;
        }
        nearesTick_--;
    }
    return 0;
}
