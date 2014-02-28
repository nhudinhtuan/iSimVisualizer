#include "viewcontroller.h"

ViewController::ViewController(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    :preferenceManager_(preferenceManager), geospatialIndex_(geospatialIndex), temporalIndex_(temporalIndex), mapView_(mapView){


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
            loadLinks();
            loadNodes();
            loadSegments();
            loadBusStops();
            loadCrossings();
            emit finishLoadingGeospatial();
            break;
        case iSimGUI::UPDATE_AGENTS:
            updateAgents();
            break;
    }
}

void ViewController::loadNodes() {
    QHash<unsigned long, MultiNode*> &multiNodes = geospatialIndex_->getMultiNodes();
    for (QHash<unsigned long, MultiNode*>::iterator it = multiNodes.begin(); it != multiNodes.end(); it++) {
        emit requestCreateGMultiNode(it.value());
    }
    QHash<unsigned long, UniNode*> &uniNodes = geospatialIndex_->getUniNodes();
    for (QHash<unsigned long, UniNode*>::iterator it = uniNodes.begin(); it != uniNodes.end(); it++) {
        emit requestCreateGUniNode(it.value());
    }
}

void ViewController::loadBusStops() {
    QHash<unsigned long, BusStop*>& busStops = geospatialIndex_->getBusStops();
    for (QHash<unsigned long, BusStop*>::iterator it = busStops.begin(); it != busStops.end(); it++) {
        emit requestCreateGBusStop(it.value());
    }
}

void ViewController::loadLinks() {
    QHash<unsigned long, Link*>& links = geospatialIndex_->getLinks();
    for (QHash<unsigned long, Link*>::iterator it = links.begin(); it != links.end(); it++) {
        Link *link = it.value();
        emit requestCreateLinkTreeItem(link);
    }
}

void ViewController::loadSegments() {
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

void ViewController::loadCrossings() {
    QHash<unsigned long, Crossing*>& crossings = geospatialIndex_->getCrossings();
    for (QHash<unsigned long, Crossing*>::iterator it = crossings.begin(); it != crossings.end(); it++) {
        Crossing *crossing = it.value();
        emit requestCreateGCrossing(crossing);
    }
}

void ViewController::updateAgents() {
    if (preferenceManager_->isMicroscopicDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getMicroscopicThreshold()) {
        QRectF sceneRect = mapView_->getGraphViewRect();
        QPoint bottomLeft(sceneRect.bottomLeft().x(), -sceneRect.bottomLeft().y());
        QPoint topRight(sceneRect.topRight().x(), -sceneRect.topRight().y());

        double marginFactor = 0.1;
        double absoluteMarginX = (topRight.x()-bottomLeft.x()) * marginFactor;
        double absoluteMarginY = (topRight.y()-bottomLeft.y()) * marginFactor;
        bottomLeft.setX(bottomLeft.x() - absoluteMarginX);
        bottomLeft.setY(bottomLeft.y() - absoluteMarginY);
        topRight.setX(topRight.x() + absoluteMarginX);
        topRight.setY(topRight.y() + absoluteMarginY);

        AgentList* agents = temporalIndex_->getAgent(bottomLeft, topRight);
        emit requestUpdateGAgents(agents);
    } else {
        emit requestRemoveGAgents();
    }
}
