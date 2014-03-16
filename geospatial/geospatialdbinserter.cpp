#include "geospatialdbinserter.h"
#define MAX_BUFFER_SIZE 5000

GeospatialDBInserter::GeospatialDBInserter(int fileId) {
    fileId_ = QString::number(fileId);


    QString nodeInsertQuery =
            "INSERT INTO \"" + fileId_ + DBManager::nodeTableName + "\""
            "(id, typeid, aimsunId, xpos, ypos)"
            "VALUES ";
    nodeInsertWorker_ = new DBInserter(QSqlDatabase::database(GEO_WRITER_CONN_NAME), nodeInsertQuery);
}

GeospatialDBInserter::~GeospatialDBInserter() {
    if (nodeInsertWorker_) {
        nodeInsertWorker_->wait();
        delete nodeInsertWorker_;
    }
}

void GeospatialDBInserter::insert(Node *node) {
    nodeInsertWorker_->insertBuffer(node->sqlInsertValue());

    if (!nodeInsertWorker_->isRunning() && nodeInsertWorker_->bufferSize() > MAX_BUFFER_SIZE) {
        insertNodeToDb();
    } else if (nodeInsertWorker_->isRunning() && nodeInsertWorker_->bufferSize() > 4 * MAX_BUFFER_SIZE) {
        insertNodeToDb();
    }
}

void GeospatialDBInserter::finishInsertingData() {
    insertNodeToDb();
    nodeInsertWorker_->wait();
}

void GeospatialDBInserter::insertNodeToDb() {
    if (nodeInsertWorker_->bufferSize() > 0) {
        if (nodeInsertWorker_->isRunning())
            nodeInsertWorker_->wait();
        nodeInsertWorker_->start();
    }
}
