#include "microscopicdatamem.h"

MicroscopicDataMem::MicroscopicDataMem()
{
}

MicroscopicDataMem::~MicroscopicDataMem() {

}

void MicroscopicDataMem::insertAgent(Agent* data) {
    unsigned int tick = data->getTick();
    if (agentTicks_.contains(tick)) {
        AgentTree *tree = agentTicks_[tick];
        tree->insert(data);
    } else {
        AgentTree *tree = new AgentTree();
        tree->insert(data);
        agentTicks_[tick] = tree;
    }
}

AgentList* MicroscopicDataMem::getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    if (agentTicks_.contains(tick)) {
        return new AgentList(agentTicks_[tick]->query(bottomLeft, topRight));
    } else {
        return new AgentList();
    }
}
