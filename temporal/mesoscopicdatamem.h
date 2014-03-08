#ifndef MESOSCOPICDATAMEM_H
#define MESOSCOPICDATAMEM_H

#include <QHash>
#include "temporal/mesoscopicdata.h"

class MesoscopicDataMem : public MesoscopicData
{
public:
    MesoscopicDataMem();
    ~MesoscopicDataMem();
    void insert(Mesoscopic* data);
    Mesoscopic* getMesoscopic(unsigned int tick, unsigned long segmentId);

private:
    QHash<unsigned int, QHash<unsigned long, Mesoscopic*> > mesoscopicTicks_;
};

#endif // MESOSCOPICDATAMEM_H
