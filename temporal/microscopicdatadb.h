#ifndef MICROSCOPICDATADB_H
#define MICROSCOPICDATADB_H

#include <QElapsedTimer>
#include <QMutex>
#include "io/dbmanager.h"
#include "io/dbinserter.h"
#include "temporal/microscopicdata.h"

class MicroscopicDataDB : public MicroscopicData
{
public:
    MicroscopicDataDB(int fileId);
    ~MicroscopicDataDB();

    void insert(CrossingPhaseData& crossingPhaseData);
    void insert(TrafficPhaseData& trafficPhaseData);
    void insert(Agent& data);
    void updatePhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);

    int getCrossingPhaseColor(unsigned int tick, unsigned long crossingId);
    TrafficPhaseData getTrafficPhaseData(unsigned int tick, unsigned long id);

    void finishInsertingData();

private:
    void insertAgentsToDB();
    void insertPhaseToDB();

    QHash<unsigned long, CrossingPhaseData> crossingPhaseData_;
    QHash<unsigned long, TrafficPhaseData> trafficPhaseData_;

    QString fileId_;
    QSqlQuery reader_;
    QMutex crossingPhaseMutex_;
    QMutex trafficPhaseMutex_;

    bool isAgentExisted_;
    DBInserter *agentInsertWorker_;
    bool isPhaseDataExisted_;
    DBInserter *phaseInsertWorker_;
};
#endif // MICROSCOPICDATADB_H
