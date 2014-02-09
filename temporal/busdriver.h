#ifndef BUSDRIVER_H
#define BUSDRIVER_H

#include "driver.h"

class BusDriver : public Driver
{
public :
    BusDriver(unsigned long id, unsigned int tick, long xPos, long yPos, double angle, double length, double width,
              int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID);
    BusDriver(Agent const* other);
    BusDriver();
    virtual ~BusDriver(){}

    unsigned long getRealArrivalTime() const { return realArrivalTime_;}
    int getDwellTime() const { return dwellTime_; }
    QString getBusLineID() const { return busLineID_; }
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_BUS; }

    virtual void copy(Agent const* other);

protected:
    unsigned long realArrivalTime_;
    int dwellTime_;
    QString busLineID_;
};
#endif // BUSDRIVER_H