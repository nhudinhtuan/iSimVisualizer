#ifndef DBLOADER_H
#define DBLOADER_H

#include <QString>
#include <QThread>
#include "dbmanager.h"
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "temporal/mesoscopic.h"
#include "temporal/agent.h"
#include "temporal/driver.h"
#include "temporal/busdriver.h"
#include "temporal/pedestrian.h"

class DBLoader: public QThread {
    Q_OBJECT

public:
    DBLoader(GeospatialIndex* geospatialIndex, TemporalIndex* temporalIndex);
    void setTarget(Record record);
    void stopLoading();

signals:
    void announceError(QString);
    void announceProgressUpdated(int);
    void announceSpatialDataFinished();
    void announceTemporalDataExists();
    void announceLog(QString);
    void announceStatus(QString);
    void announceCompleted();

protected:
    void run();

private:
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;
    QSqlQuery sql_;
    Record record_;

    bool isSpatialDataFinished_;
    bool isTemporalDataDetected_;
    bool needStop_;
};

#endif // DBLOADER_H
