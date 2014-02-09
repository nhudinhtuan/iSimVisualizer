#include "viewcontroller.h"

ViewController::ViewController(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager)
    :preferenceManager_(preferenceManager), geospatialIndex_(geospatialIndex), temporalIndex_(temporalIndex){


}

void ViewController::reset() {
    // stop all current tasks
    tasksMutex.lock();
    while(!tasks_.isEmpty()) {
        tasks_.dequeue();
    }
    tasksMutex.unlock();

}

void ViewController::run() {
    iSimGUI::ControlTaskType type;
    while (true) {
        do {
            tasksMutex.lock();
            if (tasks_.isEmpty()) {
                tasksMutex.unlock();
                break;
            }
            type = tasks_.dequeue();
            tasksMutex.unlock();
            doTask(type);
        } while (true);
        tasksMutex.lock();
        if (tasks_.isEmpty()) hasTask.wait(&tasksMutex);
        tasksMutex.unlock();
    }
}

void ViewController::addTask(iSimGUI::ControlTaskType task) {
    tasksMutex.lock();
    tasks_.enqueue(task);
    hasTask.wakeAll();
    tasksMutex.unlock();
}

void ViewController::doTask(iSimGUI::ControlTaskType task) {
    switch (task) {
        case iSimGUI::LOAD_GEOSPATIAL:
            loadNodes();
            loadSegment();
            loadBusStops();
    }
}

void ViewController::loadNodes() {
    QHash<unsigned long, MultiNode*> &multiNodes = geospatialIndex_->getMultiNodes();
    for (QHash<unsigned long, MultiNode*>::iterator it = multiNodes.begin(); it != multiNodes.end(); it++) {
        emit requestCreateGNode(it.value());
    }
    QHash<unsigned long, UniNode*> &uniNodes = geospatialIndex_->getUniNodes();
    for (QHash<unsigned long, UniNode*>::iterator it = uniNodes.begin(); it != uniNodes.end(); it++) {
        emit requestCreateGNode(it.value());
    }
}

void ViewController::loadBusStops() {
    QHash<unsigned long, BusStop*>& busStops = geospatialIndex_->getBusStops();
    for (QHash<unsigned long, BusStop*>::iterator it = busStops.begin(); it != busStops.end(); it++) {
        emit requestCreateGBusStop(it.value());
    }
}

void ViewController::loadSegment() {
    QHash<unsigned long, RoadSegment*>& roadSegments = geospatialIndex_->getRoadSegments();
    for (QHash<unsigned long, RoadSegment*>::iterator it = roadSegments.begin(); it != roadSegments.end(); it++) {
        RoadSegment *segment = it.value();
        emit requestCreateGSegment(segment);
        QVector<Lane*>& lanes = segment->getLanes();
        for (int i = 0; i < lanes.size(); ++i) {
            emit requestCreateGLane(lanes[i]);
        }
    }
}
