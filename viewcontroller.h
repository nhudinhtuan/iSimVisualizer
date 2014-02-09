#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QThread>
#include <QWaitCondition>
#include <QQueue>
#include <QHash>
#include <QDebug>
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "geospatial/g_node.h"
#include "geospatial/g_busstop.h"
#include "geospatial/g_lane.h"
#include "geospatial/g_segment.h"

namespace iSimGUI {
enum ControlTaskType {
    LOAD_GEOSPATIAL = 0
};
}

class ViewController : public QThread
{
    Q_OBJECT

public:
    ViewController(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager);
    void addTask(iSimGUI::ControlTaskType task);

    void reset();

signals:
    void requestCreateGNode(Node*);
    void requestCreateGBusStop(BusStop*);
    void requestCreateGSegment(RoadSegment*);
    void requestCreateGLane(Lane*);

protected:
    void run();
    void doTask(iSimGUI::ControlTaskType task);
    void loadNodes();
    void loadBusStops();
    void loadSegment();

private:
    PreferenceManager *preferenceManager_;
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;

    // thread tasks
    QQueue<iSimGUI::ControlTaskType> tasks_;
    QMutex tasksMutex;
    QWaitCondition hasTask;

    //graphics
};

#endif // VIEWCONTROLLER_H
