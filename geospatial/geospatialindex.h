#ifndef GEOSPATIALINDEX_H
#define GEOSPATIALINDEX_H

#include <QHash>
#include "multinode.h"
#include "uninode.h"
#include "link.h"
#include "roadsegment.h"
#include "lane.h"
#include "busstop.h"

class GeospatialIndex
{
public:
    GeospatialIndex();
    ~GeospatialIndex();
    void reset();

    QHash<unsigned long, UniNode*>& getUniNodes() {return uniNodes_;}
    QHash<unsigned long, MultiNode*>& getMultiNodes() {return multiNodes_;}
    QHash<unsigned long, BusStop*>& getBusStops() {return busStops_;}
    QHash<unsigned long, RoadSegment*>& getRoadSegments() {return roadSegments_;}
    MultiNode* getMultiNode(unsigned long);
    UniNode* getUniNode(unsigned long);
    RoadSegment* getRoadSegemnt(unsigned long);
    BusStop* getBusStop(unsigned long);

    void insert(UniNode *uniNode);
    void insert(MultiNode *multiNode);
    void insert(Link *link);
    void insert(RoadSegment *roadSegment);
    void insert(BusStop *busStop);

private:
    QHash<unsigned long, UniNode*> uniNodes_;
    QHash<unsigned long, MultiNode*> multiNodes_;
    QHash<unsigned long, Link*> links_;
    QHash<unsigned long, RoadSegment*> roadSegments_;
    QHash<unsigned long, BusStop*> busStops_;
};

#endif // GEOSPATIALINDEX_H