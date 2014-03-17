#include "dbloader.h"

DBLoader::DBLoader(GeospatialIndex* geospatialIndex, TemporalIndex* temporalIndex)
    : geospatialIndex_(geospatialIndex), temporalIndex_(temporalIndex),
      sql_(QSqlDatabase::database(GEO_CONN_NAME)){
    needStop_ = false;
}

void DBLoader::setTarget(Record record) {
    record_ = record;
    sql_.clear();
}

void DBLoader::stopLoading() {
    needStop_ = true;
}

void DBLoader::run() {
    if (record_.id == 0) return;

    emit announceStatus(tr("Loading input file .."));

    QString fileIdStr = QString::number(record_.id);
    if (!needStop_) {
        // load node
        sql_.setForwardOnly(true);
        sql_.exec("SELECT id, typeid, aimsunid, xpos, ypos FROM \"" + fileIdStr + DBManager::nodeTableName + "\"");
        unsigned long id =0, aimsunId = 0;
        double xpos = 0, ypos = 0;
        int type;
        while (!needStop_ && sql_.next()) {
            id = sql_.value(0).toULongLong();
            type = sql_.value(1).toInt();
            aimsunId = sql_.value(2).toULongLong();
            xpos = sql_.value(3).toDouble();
            ypos = -sql_.value(4).toDouble();
            if (type == UniNode::DB_ID) {
                geospatialIndex_->insert(new UniNode(id, QPointF(xpos, ypos), aimsunId));
            } else if (type == MultiNode::DB_ID) {
                geospatialIndex_->insert(new MultiNode(id, QPointF(xpos, ypos), aimsunId));
            }
        }
        sql_.clear();
    }

    // load link
    if (!needStop_) {
        sql_.setForwardOnly(true);
        sql_.exec("SELECT id, typeid, info FROM \"" + fileIdStr + DBManager::geoTableName + "\"");
        unsigned long id =0;
        int type;
        QString info;
        while (!needStop_ && sql_.next()) {
            id = sql_.value(0).toULongLong();
            type = sql_.value(1).toInt();
            info = sql_.value(2).toString();
            if (type == Link::DB_ID) {
                geospatialIndex_->insert(new Link(id, info));
            } else if (type == RoadSegment::DB_ID) {
                geospatialIndex_->insert(new RoadSegment(id, info));
            } else if (type == LaneConnector::DB_ID) {
                geospatialIndex_->insert(new LaneConnector(id, info));
            } else if (type == BusStop::DB_ID) {
                geospatialIndex_->insert(new BusStop(id, info));
            } else if (type == Crossing::DB_ID) {
                geospatialIndex_->insert(new Crossing(id, info));
            } else if (type == TrafficSignal::DB_ID) {
                geospatialIndex_->insert(new TrafficSignal(id, info));
            }
        }
        sql_.clear();
    }

    // finish
    if (needStop_) {
        emit announceLog("The file loading is aborted from user.");
    } else {
        emit announceSpatialDataFinished();

        if (record_.isMacro || record_.isMesos || record_.isMicro) {
            emit announceTemporalDataExists();
        }
        if (record_.isMesos) temporalIndex_->setMesoDataExisted();
        if (record_.isMicro) temporalIndex_->setMicroDataExisted();
        temporalIndex_->updateUniqueTicks(record_.nframe);
        emit announceCompleted();
    }

}
