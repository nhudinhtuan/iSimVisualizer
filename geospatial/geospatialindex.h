#ifndef GEOSPATIALINDEX_H
#define GEOSPATIALINDEX_H

#include <QHash>
#include <QSqlDatabase>
#include "geospatialdbinserter.h"

class GeospatialIndex
{
public:
    GeospatialIndex();
    ~GeospatialIndex();
    void reset();

    QHash<unsigned long, UniNode*>& getUniNodes() {return uniNodes_;}
    QHash<unsigned long, MultiNode*>& getMultiNodes() {return multiNodes_;}
    QHash<unsigned long, BusStop*>& getBusStops() {return busStops_;}
    QHash<unsigned long, Link*>& getLinks() {return links_;}
    QHash<unsigned long, RoadSegment*>& getRoadSegments() {return roadSegments_;}
    QHash<unsigned long, LaneConnector*>& getLaneConnectors() {return laneConnectors_;}
    QHash<unsigned long, Crossing*>& getCrossings() {return crossings_;}
    QHash<unsigned long, TrafficSignal*>& getTrafficSignals() {return trafficSignals_;}
    MultiNode* getMultiNode(unsigned long);
    UniNode* getUniNode(unsigned long);
    Link* getLink(unsigned long);
    RoadSegment* getRoadSegemnt(unsigned long);
    LaneConnector* getLaneConnector(unsigned long);
    BusStop* getBusStop(unsigned long);
    Crossing* getCrossing(unsigned long);
    TrafficSignal* getTrafficSignal(unsigned long);

    void insert(UniNode *uniNode);
    void insert(MultiNode *multiNode);
    void insert(Link *link);
    void insert(RoadSegment *roadSegment);
    void insert(LaneConnector *laneConnector);
    void insert(BusStop *busStop);
    void insert(Crossing *crossing);
    void insert(TrafficSignal *trafficSignal);

    void setWriteToDB(int fileId);
    void insertToDB();
    void waitForInsertingDB();

private:
    QHash<unsigned long, UniNode*> uniNodes_;
    QHash<unsigned long, MultiNode*> multiNodes_;
    QHash<unsigned long, Link*> links_;
    QHash<unsigned long, RoadSegment*> roadSegments_;
    QHash<unsigned long, LaneConnector*> laneConnectors_;
    QHash<unsigned long, BusStop*> busStops_;
    QHash<unsigned long, Crossing*> crossings_;
    QHash<unsigned long, TrafficSignal*> trafficSignals_;

    //db
    GeospatialDBInserter *dbInserter_;
};

#endif // GEOSPATIALINDEX_H
