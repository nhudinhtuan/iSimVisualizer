#ifndef MICROSCOPICDATA_H
#define MICROSCOPICDATA_H

#include <QPoint>
#include <QList>
#include "temporal/agent.h"
#include "temporal/driver.h"
#include "temporal/busdriver.h"
#include "temporal/pedestrian.h"
#include "temporal/agentlist.h"
#include "temporal/phasedata.h"

class MicroscopicData
{
public:
    MicroscopicData();
    virtual ~MicroscopicData();
    virtual void insert(CrossingPhaseData* crossingPhaseData) = 0;
    virtual void insert(TrafficPhaseData* trafficPhaseData) = 0;
    virtual void insert(Agent& data) = 0;
    virtual void updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) = 0;
    virtual AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) = 0;

    virtual int getCrossingPhaseColor(unsigned int tick, unsigned long crossingId) = 0;
    virtual TrafficPhaseData* getTrafficPhaseData(unsigned int tick, unsigned long id) = 0;

    virtual void finishInsertingData() = 0;
};

#endif // MICROSCOPICDATA_H
