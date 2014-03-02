#ifndef DRIVER_H
#define DRIVER_H

#include "agent.h"

class Driver: public Agent
{
public:

    Driver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length,
           double width, int passenger, int mandatory = -1, QString info = "");
    Driver();
    using Agent::update;
    virtual void update(Driver*);
    virtual ~Driver(){}

    double getLength() const { return length_; }
    double getWidth() const { return width_; }
    int getPassenger() const { return passenger_; }
    int getMandatory() const { return mandatory_;}
    QString getInfo() const { return info_;}
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_DRIVER; }

protected:
    double length_;
    double width_;
    QString info_;
    int mandatory_;
    int passenger_;
};

#endif // DRIVER_H
