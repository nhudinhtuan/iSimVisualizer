#ifndef TEMPORALINDEX_H
#define TEMPORALINDEX_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include <QList>
#include "commondef.h"
#include "io/dbio.h"
#include "preferencemanager.h"
#include "temporal/agent.h"
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
    void setUsingDB(QString dbName, iSimGUI::DataType type, bool resetDB);
    void reset();
    void updateUniqueTicks(unsigned int tick);
    unsigned int jumpToNextTick();
    bool jumpToTick(unsigned int tick);

    void insertMesoscopicData(Mesoscopic* data);
    void insertAgentData(Agent* data);
    AgentList* getAgent(QPoint& bottomLeft, QPoint& topRight);

signals:
     void announceNewUpperTickValue(unsigned int);

public slots:


private:
    PreferenceManager *preferenceManager_;
    /// Contains the complete list of unique ticks
    QList<unsigned int> uniqueTicks_;
    /// Assume that ticks are continuous in an integer-stepwise manner
    unsigned int currentTick_;

    MesoscopicData* mesoscopicData_;
    MicroscopicData* microscopicData_;
};

#endif // TEMPORALINDEX_H
