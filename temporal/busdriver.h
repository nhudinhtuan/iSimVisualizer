#ifndef BUSDRIVER_H
#define BUSDRIVER_H

#include "agent.h"

class BusDriver : public Agent
{
public :
    BusDriver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length, double width,
              int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID);
    BusDriver(Agent const* other);
    BusDriver();
    BusDriver(const BusDriver& other);
    virtual ~BusDriver(){}

    using Agent::update;
    virtual void update(BusDriver*);

    double getLength() const { return length_; }
    double getWidth() const { return width_; }
    int getPassenger() const { return passenger_;}
    unsigned long getRealArrivalTime() const { return realArrivalTime_;}
    int getDwellTime() const { return dwellTime_; }
    QString getBusLineID() const { return busLineID_; }
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_BUS; }
    QString sqlInsertValue() const;

protected:
    double length_;
    double width_;
    int passenger_;
    unsigned long realArrivalTime_;
    int dwellTime_;
    QString busLineID_;
};
#endif // BUSDRIVER_H
