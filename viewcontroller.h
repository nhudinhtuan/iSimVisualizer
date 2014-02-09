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

namespace iSimGUI {
enum ControlTaskType {
    LOAD_NODE = 1,
    LOAD_LINK = 2,
    LOAD_VERTEX= 3,
    LOAD_EDGE = 4,
    LOAD_LANE = 5,
    LOAD_BUS_STOP = 6,
    LOAD_TRAFFIC_CROSSING = 7,
    LOAD_TRAFFIC_SIGNAL = 8,
    LOAD_AGENT = 9,
    UPDATE_AGENT = 10,
    UPDATE_TRAFFIC_SIGNAL = 11
};
}

class ViewController : public QThread
{
    Q_OBJECT

public:
    ViewController(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager);
    void addTask(iSimGUI::ControlTaskType task);

    void insertGNode(unsigned long id, G_Node *gNode);
    void reset();

signals:
    void requestCreateGNode(Node*);

protected:
    void run();
    void doTask(iSimGUI::ControlTaskType task);
    void loadNodes();

private:
    PreferenceManager *preferenceManager_;
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;

    // thread tasks
    QQueue<iSimGUI::ControlTaskType> tasks_;
    QMutex tasksMutex;
    QWaitCondition hasTask;

    //graphic
    QHash<unsigned long, G_Node*> gNodes_;
};

#endif // VIEWCONTROLLER_H
