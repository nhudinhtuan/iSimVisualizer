#include "agent.h"

Agent::Agent(unsigned long id, unsigned int tick, QPointF pos, double angle, bool useDB) {
    id_ = id;
    tick_ = tick;
    pos_ = pos;
    angle_ = angle;
    useDB_ = useDB;
}

Agent::Agent() {
    id_ = 0;
    tick_ = 0;
    pos_.setX(0);
    pos_.setY(0);
    angle_ = 0;
}
