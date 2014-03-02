#ifndef AGENT_H
#define AGENT_H

#include <QString>
#include <QPointF>

namespace iSimGUI {
    enum AgentType {AGENT_DRIVER = 0, AGENT_BUS = 1, AGENT_PEDESTRIAN = 2};
}

class Agent {
public:
    Agent(unsigned long id, unsigned int tick, QPointF pos, double angle);
    Agent();
    virtual ~Agent(){}

    virtual void update(Agent*);
    unsigned long getID() const { return id_; }
    unsigned int getTick() const { return tick_; }
    QPointF& getPos() {return pos_;}
    double getAngle() const { return angle_; }
    virtual iSimGUI::AgentType getType() const = 0;

protected:
    unsigned long id_;
    unsigned int tick_;
    QPointF pos_;
    double angle_;
};

#endif // AGENT_H
