#ifndef DRIVER_H
#define DRIVER_H

#include <QVariant>
#include <QDebug>
#include "agent.h"

class Driver: public Agent
{
public:

    Driver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length,
           double width, unsigned long currentSegment, int fwdSpeed, int fwdAccel, int mandatory, QString info);
    Driver();
    Driver(const Driver& other);
    using Agent::update;
    virtual void update(Driver*);
    virtual ~Driver(){}

    double getLength() const { return length_; }
    double getWidth() const { return width_; }
    int getMandatory() const { return mandatory_;}
    QString getInfo() const { return info_;}
    unsigned long getCurrentSegment() const { return currentSegment_;}
    int getFwdSpeed() const { return fwdSpeed_;}
    int getFwdAccel() const { return fwdAccel_;}
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_DRIVER; }
    QString sqlInsertValue() const;

protected:
    double length_;
    double width_;
    QString info_;
    int mandatory_;
    unsigned long currentSegment_;
    int fwdSpeed_;
    int fwdAccel_;
};
Q_DECLARE_METATYPE(Driver)
#endif // DRIVER_H
