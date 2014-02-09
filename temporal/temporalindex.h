#ifndef TEMPORALINDEX_H
#define TEMPORALINDEX_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include "commondef.h"
#include "io/dbio.h"
#include "preferencemanager.h"
#include "temporal/mesoscopic.h"
#include "temporal/agent.h"

class TemporalIndex : public QObject
{
    Q_OBJECT

public:
    explicit TemporalIndex(QObject *parent, PreferenceManager *preferenceManager);
    ~TemporalIndex();
    void setUsingMemory();
    void setUsingDB(QString dbName, iSimGUI::DataType type, bool resetDB);
    void releaseMemory();

    void updateUniqueTicks(unsigned int tick);

    void insertMesoscopicData(Mesoscopic& mesoscopicData);
    //void insertAgentData(Agent* agentData);

signals:
     void announceUpperBoundTickValueChange(unsigned int);

public slots:


private:
    DBio* db_;
    PreferenceManager *preferenceManager_;
    /// Contains the complete list of unique ticks
    QList<unsigned int> uniqueTicks_;
    /// Assume that ticks are continuous in an integer-stepwise manner
    unsigned int currentTick_;

    //using memory
    QMutex mesoscopicMutex_;
    QHash<const unsigned long, QList<Mesoscopic*>*> mesoscopicDataIndexedBySegmentID_;



};

#endif // TEMPORALINDEX_H
