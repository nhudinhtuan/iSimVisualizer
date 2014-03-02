#ifndef BUSDRIVER_H
#define BUSDRIVER_H

#include "driver.h"

class BusDriver : public Driver
{
public :
    BusDriver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length, double width,
              int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID);
    BusDriver(Agent const* other);
    BusDriver();
    virtual ~BusDriver(){}

    using Driver::update;
    virtual void update(BusDriver*);

    unsigned long getRealArrivalTime() const { return realArrivalTime_;}
    int getDwellTime() const { return dwellTime_; }
    QString getBusLineID() const { return busLineID_; }
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_BUS; }

protected:
    unsigned long realArrivalTime_;
    int dwellTime_;
    QString busLineID_;
};
#endif // BUSDRIVER_H
