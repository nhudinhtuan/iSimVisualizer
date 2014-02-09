#ifndef GEOSPATIALINDEX_H
#define GEOSPATIALINDEX_H

#include <QHash>
#include "multinode.h"
#include "uninode.h"

class GeospatialIndex
{
public:
    GeospatialIndex();
    ~GeospatialIndex();
    void reset();

    QHash<unsigned long, UniNode*>& getUniNodes() {return uniNodes_;}
    QHash<unsigned long, MultiNode*>& getMultiNodes() {return multiNodes_;}

    void insert(UniNode *uniNode);
    void insert(MultiNode *multiNode);

private:
    QHash<unsigned long, UniNode*> uniNodes_;
    QHash<unsigned long, MultiNode*> multiNodes_;
};

#endif // GEOSPATIALINDEX_H
