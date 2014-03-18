#include "geospatialindex.h"

GeospatialIndex::GeospatialIndex()
    : dbInserter_(0){
}

GeospatialIndex::~GeospatialIndex() {
    reset();
}

void GeospatialIndex::reset() {
    if (dbInserter_) {
        if (dbInserter_->isRunning()) {
            dbInserter_->forceStop();
            dbInserter_->wait();
        }
        delete dbInserter_;
    }
    dbInserter_ = 0;

    //remove items
    QHash<unsigned long, UniNode*>::iterator uniNodeIt = uniNodes_.begin();
    while (uniNodeIt != uniNodes_.end()) {
        delete uniNodeIt.value();
        uniNodeIt++;
    }
    uniNodes_.clear();

    QHash<unsigned long, MultiNode*>::iterator mulNodeIt = multiNodes_.begin();
    while (mulNodeIt != multiNodes_.end()) {
        delete mulNodeIt.value();
        mulNodeIt++;
    }
    multiNodes_.clear();

    QHash<unsigned long, Link*>::iterator linkIt = links_.begin();
    while (linkIt != links_.end()) {
        delete linkIt.value();
        linkIt++;
    }
    links_.clear();

    QHash<unsigned long, RoadSegment*>::iterator roadSegmentIt = roadSegments_.begin();
    while (roadSegmentIt != roadSegments_.end()) {
        delete roadSegmentIt.value();
        roadSegmentIt++;
    }
    roadSegments_.clear();

    QHash<unsigned long, LaneConnector*>::iterator laneConnectorIt = laneConnectors_.begin();
    while (laneConnectorIt != laneConnectors_.end()) {
        delete laneConnectorIt.value();
        laneConnectorIt++;
    }
    laneConnectors_.clear();

    QHash<unsigned long, BusStop*>::iterator busStopIt = busStops_.begin();
    while (busStopIt != busStops_.end()) {
        delete busStopIt.value();
        busStopIt++;
    }
    busStops_.clear();

    QHash<unsigned long, Crossing*>::iterator crossingIt = crossings_.begin();
    while (crossingIt != crossings_.end()) {
        delete crossingIt.value();
        crossingIt++;
    }
    crossings_.clear();

    QHash<unsigned long, TrafficSignal*>::iterator trafficSignalIt = trafficSignals_.begin();
    while (trafficSignalIt != trafficSignals_.end()) {
        delete trafficSignalIt.value();
        trafficSignalIt++;
    }
    trafficSignals_.clear();

    QHash<unsigned long, Incident*>::iterator incidentIt = incidents_.begin();
    while (incidentIt != incidents_.end()) {
        delete incidentIt.value();
        incidentIt++;
    }
    incidents_.clear();

    aimsunIDToSegmentId_.clear();
}


void GeospatialIndex::insert(UniNode *uniNode) {
    uniNodes_[uniNode->getId()] = uniNode;
}
void GeospatialIndex::insert(MultiNode *multiNode) {
    multiNodes_[multiNode->getId()] = multiNode;
}
void GeospatialIndex::insert(Link *link) {
    links_[link->getId()] = link;
}
void GeospatialIndex::insert(RoadSegment *roadSegment) {
    roadSegments_[roadSegment->getId()] = roadSegment;
    aimsunIDToSegmentId_[roadSegment->getAimsunId()] = roadSegment->getId();
}
void GeospatialIndex::insert(LaneConnector *laneConnector) {
    laneConnectors_[laneConnector->getId()] = laneConnector;
}
void GeospatialIndex::insert(BusStop *busStop) {
    busStops_[busStop->getId()] = busStop;
}

void GeospatialIndex::insert(Crossing *crossing) {
    crossings_[crossing->getId()] = crossing;
}

void GeospatialIndex::insert(TrafficSignal *trafficSignal) {
    trafficSignals_[trafficSignal->getId()] = trafficSignal;
}

void GeospatialIndex::insert(Incident *incident) {
    incidents_[incident->getId()] = incident;
}

MultiNode* GeospatialIndex::getMultiNode(unsigned long id) {
    if (multiNodes_.contains(id)) return multiNodes_[id];
    return 0;
}

UniNode* GeospatialIndex::getUniNode(unsigned long id) {
    if (uniNodes_.contains(id)) return uniNodes_[id];
    return 0;
}

Link* GeospatialIndex::getLink(unsigned long id) {
    if (links_.contains(id)) return links_[id];
    return 0;
}

RoadSegment* GeospatialIndex::getRoadSegemnt(unsigned long id) {
    if (roadSegments_.contains(id)) return roadSegments_[id];
    return 0;
}

RoadSegment* GeospatialIndex::getSegmentByAimSunId(unsigned long aimsunId) {
    unsigned long segmentId =  aimsunIDToSegmentId_.value(aimsunId);
    RoadSegment* roadSegment = 0;
    if (segmentId) {
        roadSegment = getRoadSegemnt(segmentId);
    }
    return roadSegment;
}

LaneConnector* GeospatialIndex::getLaneConnector(unsigned long id) {
    if (laneConnectors_.contains(id)) return laneConnectors_[id];
    return 0;
}

BusStop* GeospatialIndex::getBusStop(unsigned long id) {
    if (busStops_.contains(id)) return busStops_[id];
    return 0;
}

Crossing* GeospatialIndex::getCrossing(unsigned long id) {
    if (crossings_.contains(id)) return crossings_[id];
    return 0;
}

TrafficSignal* GeospatialIndex::getTrafficSignal(unsigned long id) {
    if(trafficSignals_.contains(id)) return trafficSignals_[id];
    return 0;
}

Incident* GeospatialIndex::getIncident(unsigned long id) {
    if (incidents_.contains(id)) return incidents_[id];
    return 0;
}

void GeospatialIndex::setWriteToDB(int fileId) {
    if (dbInserter_) {
        if (dbInserter_->isRunning()) {
            dbInserter_->forceStop();
            dbInserter_->wait();
        }
        delete dbInserter_;
    }
    dbInserter_ = new GeospatialDBInserter(fileId);
}

void GeospatialIndex::insertToDB() {
    if (!dbInserter_) return;
    dbInserter_->setData(&uniNodes_, &multiNodes_, &links_, &roadSegments_,
                         &laneConnectors_, &busStops_, &crossings_, &trafficSignals_, &incidents_);
    dbInserter_->start();
}

void GeospatialIndex::waitForInsertingDB() {
    if (dbInserter_ && dbInserter_->isRunning()) {
        dbInserter_->wait();
    }
}
