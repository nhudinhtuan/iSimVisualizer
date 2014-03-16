#ifndef GEOSPATIALDBINSERTER_H
#define GEOSPATIALDBINSERTER_H

#include "io/dbmanager.h"
#include "io/dbinserter.h"
#include "geospatial/node.h"

class GeospatialDBInserter
{

public:
    GeospatialDBInserter(int fileId);
    ~GeospatialDBInserter();

    void insert(Node *node);
    void finishInsertingData();

private:
    void insertNodeToDb();

    QString fileId_;
    DBInserter *nodeInsertWorker_;
};

#endif // GEOSPATIALDBINSERTER_H
