#ifndef MESOSCOPICDATA_H
#define MESOSCOPICDATA_H

#include "temporal/mesoscopic.h"

class MesoscopicData
{
public:
    MesoscopicData();
    virtual ~MesoscopicData();
    virtual void insert(Mesoscopic* data) = 0;
};

#endif // MESOSCOPICDATA_H
