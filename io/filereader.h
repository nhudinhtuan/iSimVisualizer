#ifndef FILEREADER_H
#define FILEREADER_H

#include <QFile>
#include <QHash>
#include <QMap>
#include <QObject>
#include <QRegExp>
#include <QString>
#include <QThread>
#include <QStringList>
#include <QTextStream>
#include <QDebug>
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "temporal/mesoscopic.h"
#include "temporal/agent.h"
#include "temporal/driver.h"
#include "temporal/busdriver.h"
#include "temporal/pedestrian.h"
#include "util/json.h"

class FileReader : public QThread {
    Q_OBJECT

public:
    FileReader(GeospatialIndex* geospatialIndex, TemporalIndex* temporalIndex);
    ~FileReader();
    void setTarget(QString path);
    void stopReader();

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
    bool processLine(QString &line);
    QPointF genCoordinate(double x, double y);

    bool createMesoscopic(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties);
    bool createAgent(const QString& objType, unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties);
    bool createDriver(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties);
    bool createBusDriver(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties);
    bool createPedestrian(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties);

    bool parseRoadNetworkDataLine(const QString &objType, unsigned long objID, QMap<QString, QString> &properties);
    bool createMultiNode(unsigned long id, QMap<QString, QString> &properties);
    bool createUniNode(unsigned long id, QMap<QString, QString> &properties);
    bool createLink(unsigned long id, QMap<QString, QString> &properties);
    bool createRoadSegment(unsigned long id, QMap<QString, QString> &properties);
    bool createPolyline(unsigned long id, QMap<QString, QString> &properties);
    bool createLane(QMap<QString, QString> &properties);
    bool createLaneConnector(unsigned long id, QMap<QString, QString> &properties);
    bool createGraph(unsigned long id, QMap<QString, QString> &properties);
    bool createVertex(unsigned long id, QMap<QString, QString> &properties);
    bool createEdge(unsigned long id, QMap<QString, QString> &properties);
    bool createBusstop(unsigned long id, QMap<QString, QString> &properties);
    bool createCrossing(unsigned long id, QMap<QString, QString> &properties);
    bool createTrafficSignal(QVariantMap properties);

    bool createPhaseData(QVariantMap properties);

private:
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;
    QFile *fileHandle_;

    QString path_;
    bool isSpatialDataFinished_;
    bool isTemporalDataDetected_;
    bool needStop_;
};


#endif // FILEREADER_H
