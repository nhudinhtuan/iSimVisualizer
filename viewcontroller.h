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
#include "geospatial/g_uninode.h"
#include "geospatial/g_multinode.h"
#include "geospatial/g_busstop.h"
#include "geospatial/g_lane.h"
#include "geospatial/g_segment.h"
#include "geospatial/g_crossing.h"
#include "temporal/g_agent.h"

namespace iSimGUI {
enum ControlTaskType {
    LOAD_GEOSPATIAL = 0,
    UPDATE_AGENTS = 1
};
}

class ViewController : public QThread
{
    Q_OBJECT

public:
    ViewController(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    void addTask(iSimGUI::ControlTaskType task);
    void reset();

signals:
    void requestCreateGUniNode(UniNode*);
    void requestCreateGMultiNode(MultiNode*);
    void requestCreateGBusStop(BusStop*);
    void requestCreateGSegment(RoadSegment*);
    void requestCreateGLane(Lane*);
    void requestCreateGCrossing(Crossing*);
    void requestCreateLinkTreeItem(Link*);
    void requestUpdateGAgents(AgentList*);
    void requestRemoveGAgents();
    void finishLoadingGeospatial();

protected:
    void run();
    void doTask(iSimGUI::ControlTaskType task);
    void loadNodes();
    void loadBusStops();
    void loadLinks();
    void loadSegments();
    void loadCrossings();
    void updateAgents();

private:
    PreferenceManager *preferenceManager_;
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;
    MapGraphicsView *mapView_;

    // thread tasks
    QQueue<iSimGUI::ControlTaskType> tasks_;
    QMutex tasksMutex;
    QWaitCondition hasTask;

    //graphics
};

#endif // VIEWCONTROLLER_H
