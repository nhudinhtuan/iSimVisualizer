#ifndef MICROSCOPICDATADB_H
#define MICROSCOPICDATADB_H

#include "temporal/microscopicdata.h"

class MicroscopicDataDB : public MicroscopicData
{
public:
    MicroscopicDataDB();
    ~MicroscopicDataDB();

    void insert(CrossingPhaseData* crossingPhaseData);
    void insert(TrafficPhaseData* trafficPhaseData);
    void insert(Agent* data);
    void updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);

    int getCrossingPhaseColor(unsigned int tick, unsigned long crossingId);
    TrafficPhaseData* getTrafficPhaseData(unsigned int tick, unsigned long id);
};

#endif // MICROSCOPICDATADB_H
