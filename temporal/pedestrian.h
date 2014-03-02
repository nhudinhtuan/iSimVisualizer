#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "agent.h"

class Pedestrian: public Agent
{
public:
    Pedestrian(unsigned long id, unsigned int tick, QPointF pos):
        Agent(id, tick, pos, 0){}
    Pedestrian(){}
    virtual ~Pedestrian() {}

    using Agent::update;
    void update(Pedestrian* pedestrian);
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_PEDESTRIAN; }
};


#endif // PEDESTRIAN_H
