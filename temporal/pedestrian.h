#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include "agent.h"

class Pedestrian: public Agent
{
public:
    Pedestrian(unsigned long id, unsigned int tick, long xPos, long yPos):
        Agent(id, tick, xPos, yPos, 0){}
    Pedestrian(Agent const* other) { copy(other); }
    Pedestrian(){}
    virtual ~Pedestrian() {}

    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_PEDESTRIAN; }
    void copy(Agent const* other) { Agent::copy(other);}
};


#endif // PEDESTRIAN_H
