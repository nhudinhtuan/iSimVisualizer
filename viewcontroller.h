#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QThread>
#include <QWaitCondition>
#include <QQueue>
#include <QHash>
#include <QElapsedTimer>
#include <QDebug>
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "geospatial/g_node.h"
#include "geospatial/g_uninode.h"
#include "geospatial/g_multinode.h"
#include "geospatial/g_busstop.h"
#include "geospatial/g_lane.h"
#include "geospatial/g_laneconnector.h"
#include "geospatial/g_segment.h"
#include "geospatial/g_crossing.h"
#include "geospatial/g_trafficsignal.h"
#include "geospatial/g_incident.h"
#include "temporal/g_agent.h"
#include "temporal/g_driver.h"
#include "temporal/g_busdriver.h"
#include "temporal/g_pedestrian.h"

namespace iSimGUI {
enum ControlTaskType {
    LOAD_GEOSPATIAL = 0,
    UPDATE_AGENTS = 1,
    UPDATE_MICRO_DATA = 2,
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
    void requestCreateGLaneConnector(LaneConnector*);
    void requestCreateGCrossing(Crossing*);
    void requestCreateGTrafficSignal(TrafficSignal*);
    void requestCreateGIncident(Incident*);
    void requestCreateLinkTreeItem(Link*);
    void requestUpdateGAgents(AgentList*);
    void finishLoadingGeospatial();
    void finishUpdateDynamic();

protected:
    void run();
    void doTask(iSimGUI::ControlTaskType task);
    void loadNodes();
    void loadBusStops();
    void loadLinks();
    void loadSegments();
    void loadLaneConnectors();
    void loadCrossings();
    void loadTrafficSignal();
    void loadIncidents();
    void updateAgents(QPoint& bottomLeft, QPoint& topRight);
    void updateTrafficSignalData(QPoint& bottomLeft, QPoint& topRight);

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
