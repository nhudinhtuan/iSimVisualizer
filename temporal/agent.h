#ifndef AGENT_H
#define AGENT_H

#include "commondef.h"

class Agent {
public:
    Agent(unsigned long id, unsigned int tick, long xPos, long yPos, double angle, bool useDB = true);
    Agent();
    virtual ~Agent(){}

    unsigned long getID() const { return id_; }
    unsigned int getTick() const { return tick_; }
    long getXPos() const { return xPos_; }
    long getYPos() const { return yPos_; }
    long getUseDB() const { return useDB_; }
    double getAngle() const { return angle_; }
    virtual iSimGUI::AgentType getType() const = 0;
    void setUseDB(bool useDB) { useDB_ = useDB; }

    virtual void copy(Agent const* other);
    void updateData(unsigned int tick, long xPos, long yPos, double angle);

protected:
    unsigned long id_;
    unsigned int tick_;
    long xPos_;
    long yPos_;
    double angle_;
    bool useDB_;
};

#endif // AGENT_H
