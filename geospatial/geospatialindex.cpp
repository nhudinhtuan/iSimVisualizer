#include "geospatialindex.h"

GeospatialIndex::GeospatialIndex() {
}

GeospatialIndex::~GeospatialIndex() {
    reset();
}

void GeospatialIndex::reset() {
    //remove items
    QHash<unsigned long, UniNode*>::iterator uniNodeIt = uniNodes_.begin();
    while (uniNodeIt != uniNodes_.end()) {
        delete uniNodeIt.value();
        uniNodeIt = uniNodes_.erase(uniNodeIt);
    }
    QHash<unsigned long, MultiNode*>::iterator mulNodeIt = multiNodes_.begin();
    while (mulNodeIt != multiNodes_.end()) {
        delete mulNodeIt.value();
        mulNodeIt = multiNodes_.erase(mulNodeIt);
    }
    QHash<unsigned long, Link*>::iterator linkIt = links_.begin();
    while (linkIt != links_.end()) {
        delete linkIt.value();
        linkIt = links_.erase(linkIt);
    }
    QHash<unsigned long, RoadSegment*>::iterator roadSegmentIt = roadSegments_.begin();
    while (roadSegmentIt != roadSegments_.end()) {
        delete roadSegmentIt.value();
        roadSegmentIt = roadSegments_.erase(roadSegmentIt);
    }
    QHash<unsigned long, BusStop*>::iterator busStopIt = busStops_.begin();
    while (busStopIt != busStops_.end()) {
        delete busStopIt.value();
        busStopIt = busStops_.erase(busStopIt);
    }
    QHash<unsigned long, Crossing*>::iterator crossingIt = crossings_.begin();
    while (crossingIt != crossings_.end()) {
        delete crossingIt.value();
        crossingIt = crossings_.erase(crossingIt);
    }
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
}
void GeospatialIndex::insert(BusStop *busStop) {
    busStops_[busStop->getId()] = busStop;
}

void GeospatialIndex::insert(Crossing *crossing) {
    crossings_[crossing->getId()] = crossing;
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

BusStop* GeospatialIndex::getBusStop(unsigned long id) {
    if (busStops_.contains(id)) return busStops_[id];
    return 0;
}

Crossing* GeospatialIndex::getCrossing(unsigned long id) {
    if (crossings_.contains(id)) return crossings_[id];
    return 0;
}
