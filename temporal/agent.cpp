#include "agent.h"

Agent::Agent(unsigned long id, unsigned int tick, QPointF pos, double angle) {
    id_ = id;
    tick_ = tick;
    pos_ = pos;
    angle_ = angle;
}

Agent::Agent() {
    id_ = 0;
    tick_ = 0;
    pos_.setX(0);
    pos_.setY(0);
    angle_ = 0;
}

void Agent::update(Agent* data) {
    id_ = data->id_;
    tick_ = data->tick_;
    pos_ = data->pos_;
    angle_ = data->angle_;
}
