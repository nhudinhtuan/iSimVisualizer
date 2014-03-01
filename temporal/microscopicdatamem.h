#ifndef MICROSCOPICDATAMEM_H
#define MICROSCOPICDATAMEM_H

#include <QHash>
#include "temporal/microscopicdata.h"
#include "rstart_tree/agenttree.h"

class MicroscopicDataMem : public MicroscopicData
{
public:
    MicroscopicDataMem();
    ~MicroscopicDataMem();

    void insert(CrossingPhaseData* crossingPhaseData);
    void insert(TrafficPhaseData* trafficPhaseData);
    void insert(Agent* data);
    void updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);

    int getCrossingPhaseColor(unsigned int tick, unsigned long crossingId);
    TrafficPhaseData* getTrafficPhaseData(unsigned int tick, unsigned long id);
private:
    QHash<unsigned int, AgentTree*> agentTicks_;
    QHash<unsigned int, QHash<unsigned long, CrossingPhaseData*> > crossingPhaseData_;
    QHash<unsigned int, QHash<unsigned long, TrafficPhaseData*> > trafficPhaseData_;
};

#endif // MICROSCOPICDATAMEM_H
