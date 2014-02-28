#ifndef MICROSCOPICDATADB_H
#define MICROSCOPICDATADB_H

#include "temporal/microscopicdata.h"

class MicroscopicDataDB : public MicroscopicData
{
public:
    MicroscopicDataDB();
    ~MicroscopicDataDB();

    void insertAgent(Agent* data);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
};

#endif // MICROSCOPICDATADB_H
