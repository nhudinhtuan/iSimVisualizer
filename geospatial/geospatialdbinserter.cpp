#include "geospatialdbinserter.h"
#define MAX_BUFFER_SIZE 10000
#define MAX_SEGMENT_BUFFER_SIZE 8000

GeospatialDBInserter::GeospatialDBInserter(int fileId)
    : uniNodes_(0), multiNodes_(0), links_(0), roadSegments_(0), laneConnectors_(0), busStops_(0),
      crossings_(0), trafficSignals_(0), sql_(QSqlDatabase::database(GEO_CONN_NAME)){

    fileId_ = QString::number(fileId);
    forceStop_ = false;
}

GeospatialDBInserter::~GeospatialDBInserter() {
    sql_.clear();
    sql_.finish();
}

void GeospatialDBInserter::forceStop() {
    forceStop_ = true;
}

void GeospatialDBInserter::setData(QHash<unsigned long, UniNode*>* uniNodes,
             QHash<unsigned long, MultiNode*>* multiNodes,
             QHash<unsigned long, Link*>* links,
             QHash<unsigned long, RoadSegment*>* roadSegments,
             QHash<unsigned long, LaneConnector*>* laneConnectors,
             QHash<unsigned long, BusStop*>* busStops,
             QHash<unsigned long, Crossing*>* crossings,
             QHash<unsigned long, TrafficSignal*>* trafficSignals,
             QHash<unsigned long, Incident*>* incidents) {
    uniNodes_ = uniNodes;
    multiNodes_ = multiNodes;
    links_ = links;
    roadSegments_ = roadSegments;
    laneConnectors_ = laneConnectors;
    busStops_ = busStops;
    crossings_ = crossings;
    trafficSignals_ = trafficSignals;
    incidents_ = incidents;
}

void GeospatialDBInserter::run() {
    QStringList buffer;
    unsigned int count;

    // insert uninodes
    QHash<unsigned long, UniNode*>::iterator uniNodeIt = uniNodes_->begin();
    count = 0;
    buffer.clear();
    while (uniNodeIt != uniNodes_->end() && !forceStop_) {
        count++;
        buffer << uniNodeIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertNodeToDB(buffer);
            buffer.clear();
            count = 0;
        }
        uniNodeIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertNodeToDB(buffer);

    // insert multinodes
    QHash<unsigned long, MultiNode*>::iterator mulNodeIt = multiNodes_->begin();
    count = 0;
    buffer.clear();
    while (mulNodeIt != multiNodes_->end() && !forceStop_) {
        count++;
        buffer << mulNodeIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertNodeToDB(buffer);
            buffer.clear();
            count = 0;
        }
        mulNodeIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertNodeToDB(buffer);

    // insert links
    QHash<unsigned long, Link*>::iterator linkIt = links_->begin();
    count = 0;
    buffer.clear();
    while (linkIt != links_->end() && !forceStop_) {
        count++;
        buffer << linkIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        linkIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // insert segment
    QHash<unsigned long, RoadSegment*>::iterator roadSegmentIt = roadSegments_->begin();
    count = 0;
    buffer.clear();
    while (roadSegmentIt != roadSegments_->end() && !forceStop_) {
        count++;
        buffer << roadSegmentIt.value()->sqlInsertValue();
        if (count > MAX_SEGMENT_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        roadSegmentIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // lane connector
    QHash<unsigned long, LaneConnector*>::iterator laneConnectorIt = laneConnectors_->begin();
    count = 0;
    buffer.clear();
    while (laneConnectorIt != laneConnectors_->end() && !forceStop_) {
        count++;
        buffer << laneConnectorIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        laneConnectorIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // bus stop
    QHash<unsigned long, BusStop*>::iterator busStopIt = busStops_->begin();
    count = 0;
    buffer.clear();
    while (busStopIt != busStops_->end() && !forceStop_) {
        count++;
        buffer << busStopIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        busStopIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // crossing
    QHash<unsigned long, Crossing*>::iterator crossingIt = crossings_->begin();
    count = 0;
    buffer.clear();
    while (crossingIt != crossings_->end() && !forceStop_) {
        count++;
        buffer << crossingIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        crossingIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // traffic signal
    QHash<unsigned long, TrafficSignal*>::iterator trafficSignalIt = trafficSignals_->begin();
    count = 0;
    buffer.clear();
    while (trafficSignalIt != trafficSignals_->end() && !forceStop_) {
        count++;
        buffer << trafficSignalIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        trafficSignalIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);

    // incident
    QHash<unsigned long, Incident*>::iterator incidentIt = incidents_->begin();
    count = 0;
    buffer.clear();
    while (incidentIt != incidents_->end() && !forceStop_) {
        count++;
        buffer << incidentIt.value()->sqlInsertValue();
        if (count > MAX_BUFFER_SIZE) {
            insertGeoToDB(buffer);
            buffer.clear();
            count = 0;
        }
        incidentIt++;
    }
    if (forceStop_) return;
    if (count > 0) insertGeoToDB(buffer);
}

void GeospatialDBInserter::insertNodeToDB(QStringList& buffer) {
    QString preQuery =
            "INSERT INTO \"" + fileId_ + DBManager::nodeTableName + "\""
            "(id, typeid, aimsunid, xpos, ypos)"
            "VALUES ";
    QString completeQuery = preQuery + buffer.join(",");
    if (!sql_.exec(completeQuery))
        qDebug() << sql_.lastError().text();
    sql_.clear();
}

void GeospatialDBInserter::insertGeoToDB(QStringList& buffer) {
    QString preQuery =
            "INSERT INTO \"" + fileId_ + DBManager::geoTableName + "\""
            "(id, typeid, info)"
            "VALUES ";
    QString completeQuery = preQuery + buffer.join(",");
    if (!sql_.exec(completeQuery))
        qDebug() << sql_.lastError().text();
    sql_.clear();
}
