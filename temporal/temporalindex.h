#ifndef TEMPORALINDEX_H
#define TEMPORALINDEX_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include <QList>
#include "preferencemanager.h"
#include "temporal/agent.h"
#include "temporal/mesoscopic.h"
#include "temporal/mesoscopicdatamem.h"
#include "temporal/mesoscopicdatadb.h"
#include "temporal/microscopicdatamem.h"
#include "temporal/microscopicdatadb.h"


class TemporalIndex : public QObject
{
    Q_OBJECT

public:
    explicit TemporalIndex(QObject *parent, PreferenceManager *preferenceManager);
    ~TemporalIndex();
    void setUsingMemory();
    void setUsingDB(int fileId);
    void reset();
    void updateUniqueTicks(unsigned int tick);
    unsigned int getMaxTick() { return maxTick_;}
    unsigned int getCurrentTick() { return currentTick_;}
    unsigned int jumpToNextTick();
    bool jumpToTick(unsigned int tick);
    bool isMesoDataExisted() { return mesoDataExist_;}
    void setMesoDataExisted() { mesoDataExist_ = true;}
    bool isMicroDataExisted() { return microDataExist_;}
    void setMicroDataExisted();

    void insert(Mesoscopic* data);
    void insert(Agent& data);
    void insert(CrossingPhaseData& crossingPhaseData);
    void insert(TrafficPhaseData& trafficPhaseData);
    AgentList* getAgent(QPoint& bottomLeft, QPoint& topRight);
    int getCrossingPhaseColor(unsigned long crossingId);
    TrafficPhaseData getTrafficPhaseData(unsigned long id);
    Mesoscopic* getMesoscopic(unsigned long segmentId);

    void updateCrossingPhaseData(QPoint& bottomLeft, QPoint& topRight);
    void finishInsertingData();

signals:
    void announceNewUpperTickValue(unsigned int);
    void announceMesoscopicDataExist();
    void announceMicroDataExist();

public slots:


private:
    PreferenceManager *preferenceManager_;
    /// Contains the complete list of unique ticks
    unsigned int maxTick_;
    /// Assume that ticks are continuous in an integer-stepwise manner
    unsigned int currentTick_;

    MesoscopicData* mesoscopicData_;
    MicroscopicData* microscopicData_;

    bool microDataExist_;
    bool mesoDataExist_;
};

#endif // TEMPORALINDEX_H
