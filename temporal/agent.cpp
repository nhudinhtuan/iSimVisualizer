#include "agent.h"

Agent::Agent(unsigned long id, unsigned int tick, long xPos, long yPos, double angle, bool useDB) {
    id_ = id;
    tick_ = tick;
    xPos_ = xPos;
    yPos_ = yPos;
    angle_ = angle;
    useDB_ = useDB;
}

Agent::Agent() {
    id_ = 0;
    tick_ = 0;
    xPos_ = 0;
    yPos_ = 0;
    angle_ = 0;
}

void Agent::copy(Agent const* other) {
    id_ = other->getID();
    tick_ = other->getTick();
    xPos_ = other->getXPos();
    yPos_ = other->getYPos();
    angle_ = other->getAngle();
}

void Agent::updateData(unsigned int tick, long xPos, long yPos, double angle) {
    tick_ = tick;
    xPos_ = xPos;
    yPos_ = yPos;
    angle_ = angle;
}
