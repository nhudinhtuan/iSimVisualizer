#ifndef MICROSCOPICDATADB_H
#define MICROSCOPICDATADB_H

#include "io/dbmanager.h"
#include "temporal/microscopicdata.h"

class MicroscopicDataDB : public MicroscopicData
{
public:
    MicroscopicDataDB(int fileId);
    ~MicroscopicDataDB();

    void insert(CrossingPhaseData* crossingPhaseData);
    void insert(TrafficPhaseData* trafficPhaseData);
    void insert(Agent& data);
    void updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);
    AgentList* getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight);

    int getCrossingPhaseColor(unsigned int tick, unsigned long crossingId);
    TrafficPhaseData* getTrafficPhaseData(unsigned int tick, unsigned long id);

    void finishInsertingData();

private:
    void insertAgentsToDB();

    QSqlQuery *inserter_;
    QSqlQuery *reader_;
    QString fileId_;

    int countAgentRecords_;
    QStringList driverBuffer_;
    QStringList busBuffer_;
    QStringList pedestrianBuffer_;

    bool isDriverExisted_;
    bool isBusExisted_;
    bool isPedestrianExisted_;
};
#endif // MICROSCOPICDATADB_H
