#ifndef MICROSCOPICDATA_H
#define MICROSCOPICDATA_H

#include <QPoint>
#include <QList>
#include "temporal/agent.h"
#include "temporal/agentlist.h"

class MicroscopicData
{
public:
    MicroscopicData();
    virtual ~MicroscopicData();
    virtual void insertAgent(Agent* data) = 0;
    virtual AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) = 0;
};

#endif // MICROSCOPICDATA_H
