#ifndef GEOSPATIALDBINSERTER_H
#define GEOSPATIALDBINSERTER_H

#include <QThread>
#include "io/dbmanager.h"
#include "node.h"
#include "multinode.h"
#include "uninode.h"
#include "link.h"
#include "roadsegment.h"
#include "lane.h"
#include "laneconnector.h"
#include "busstop.h"
#include "crossing.h"
#include "trafficsignal.h"
#include "incident.h"

class GeospatialDBInserter : public QThread {
    Q_OBJECT

public:
    GeospatialDBInserter(int fileId);
    ~GeospatialDBInserter();

    void setData(QHash<unsigned long, UniNode*>* uniNodes,
                 QHash<unsigned long, MultiNode*>* multiNodes,
                 QHash<unsigned long, Link*>* links,
                 QHash<unsigned long, RoadSegment*>* roadSegments,
                 QHash<unsigned long, LaneConnector*>* laneConnectors,
                 QHash<unsigned long, BusStop*>* busStops,
                 QHash<unsigned long, Crossing*>* crossings,
                 QHash<unsigned long, TrafficSignal*>* trafficSignals,
                 QHash<unsigned long, Incident*>* incidents);
    void forceStop();

protected:
    void run();

private:
    void insertNodeToDB(QStringList& buffer);
    void insertGeoToDB(QStringList& buffer);

    QString fileId_;
    QHash<unsigned long, UniNode*>* uniNodes_;
    QHash<unsigned long, MultiNode*>* multiNodes_;
    QHash<unsigned long, Link*>* links_;
    QHash<unsigned long, RoadSegment*>* roadSegments_;
    QHash<unsigned long, LaneConnector*>* laneConnectors_;
    QHash<unsigned long, BusStop*>* busStops_;
    QHash<unsigned long, Crossing*>* crossings_;
    QHash<unsigned long, TrafficSignal*>* trafficSignals_;
    QHash<unsigned long, Incident*>* incidents_;

    QSqlQuery sql_;
    bool forceStop_;
};

#endif // GEOSPATIALDBINSERTER_H
