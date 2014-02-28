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

    void insertAgent(Agent* data);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
private:
    QHash<unsigned int, AgentTree*> agentTicks_;
};

#endif // MICROSCOPICDATAMEM_H
