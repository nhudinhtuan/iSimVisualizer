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
    if (task == iSimGUI::LOAD_GEOSPATIAL) {
        loadLinks();
        loadNodes();
        loadSegments();
        loadBusStops();
        loadCrossings();
        loadLaneConnectors();
        loadTrafficSignal();
        emit finishLoadingGeospatial();
    } else {
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

            if (task == iSimGUI::UPDATE_AGENTS) {
                updateAgents(bottomLeft, topRight);
            } else if (task == iSimGUI::UPDATE_MICRO_DATA) {
                updateTrafficSignalData(bottomLeft, topRight);
                updateAgents(bottomLeft, topRight);
            }
        } else {
            emit requestUpdateGAgents(0);
        }
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

void ViewController::loadLaneConnectors() {
    QHash<unsigned long, LaneConnector*>& laneConnectors = geospatialIndex_->getLaneConnectors();
    for (QHash<unsigned long, LaneConnector*>::iterator it = laneConnectors.begin(); it != laneConnectors.end(); it++) {
        LaneConnector *laneConnector = it.value();
        RoadSegment* fromSegment = geospatialIndex_->getRoadSegemnt(laneConnector->getFromSegment());
        RoadSegment* toSegment = geospatialIndex_->getRoadSegemnt(laneConnector->getToSegment());
        if (!fromSegment || !toSegment) continue;

        Lane* fromLane1 = fromSegment->getLane(laneConnector->getFromLane());
        Lane* fromLane2 = fromSegment->getLane(laneConnector->getFromLane()+1);
        Lane* toLane1 = toSegment->getLane(laneConnector->getToLane());
        Lane* toLane2 = toSegment->getLane(laneConnector->getToLane()+1);
        if (!fromLane1 || !fromLane2 || !toLane1 || !toLane2) continue;

        QPointF fromPoint = (fromLane1->getLastPoint() + fromLane2->getLastPoint())/2;
        QPointF toPoint = (toLane1->getFirstPoint() + toLane2->getFirstPoint())/2;
        laneConnector->setPoints(fromPoint, toPoint);
        emit requestCreateGLaneConnector(laneConnector);
    }
}

void ViewController::loadCrossings() {
    QHash<unsigned long, Crossing*>& crossings = geospatialIndex_->getCrossings();
    for (QHash<unsigned long, Crossing*>::iterator it = crossings.begin(); it != crossings.end(); it++) {
        Crossing *crossing = it.value();
        emit requestCreateGCrossing(crossing);
    }
}


void ViewController::loadTrafficSignal() {
    QHash<unsigned long, TrafficSignal*>& trafficSignals = geospatialIndex_->getTrafficSignals();
    for (QHash<unsigned long, TrafficSignal*>::iterator it = trafficSignals.begin(); it != trafficSignals.end(); it++) {
        TrafficSignal *trafficSignal = it.value();
        QList<TrafficPhase*> phases = trafficSignal->getPhases();
        for (QList<TrafficPhase*>::iterator phaseIt = phases.begin(); phaseIt != phases.end(); ++phaseIt) {
            TrafficPhase* phase = *phaseIt;
            for (int i = 0; i < phase->segments.size(); i++) {
                QPair<unsigned long, unsigned long>& segment = phase->segments[i];
                RoadSegment* fromSegment = geospatialIndex_->getRoadSegemnt(segment.first);
                RoadSegment* toSegment = geospatialIndex_->getRoadSegemnt(segment.second);
                if (!fromSegment || !toSegment) {
                    phase->lines.append(QPair<QPointF, QPointF>(QPointF(0,0), QPointF(0,0)));
                } else {
                    //for every pair of road segments, the whole process is repeated (a line with dashed arrows drawn)
                    //the middle Lane is chosen in each road segment, between which the line is drawn
                    Lane* fromLane = fromSegment->getMiddleLane();
                    Lane* toLane = toSegment->getMiddleLane();
                    phase->lines.append(QPair<QPointF, QPointF>(fromLane->getLastPoint(), toLane->getFirstPoint()));
                }
            }
        }
        emit requestCreateGTrafficSignal(trafficSignal);
    }
}


void ViewController::updateTrafficSignalData(QPoint& bottomLeft, QPoint& topRight) {
    /*
    qDebug() << "-------";
    QList<QGraphicsItem*> items = mapView_->getSeenItems();
    for (QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); ++i) {
        G_UniNode* item = dynamic_cast<G_UniNode*>(*i);
        if (item) {
            qDebug() << item->getModelId();
        }
    }*/
    temporalIndex_->updateCrossingPhaseData(bottomLeft, topRight);
}

void ViewController::updateAgents(QPoint& bottomLeft, QPoint& topRight) {
    AgentList* agents = temporalIndex_->getAgent(bottomLeft, topRight);
    emit requestUpdateGAgents(agents);
}
