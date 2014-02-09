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

    //graphical items are remove by scene, just clear pointer from hash
    gNodes_.clear();
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
        case iSimGUI::LOAD_NODE:
            loadNodes();
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


void ViewController::insertGNode(unsigned long id, G_Node *gNode) {
    gNodes_[id] = gNode;
}
