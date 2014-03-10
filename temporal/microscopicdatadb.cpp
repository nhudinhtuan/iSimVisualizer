#include "microscopicdatadb.h"
#define MAX_BUFFER_SIZE 100000

MicroscopicDataDB::MicroscopicDataDB(int fileId) {
    fileId_ = QString::number(fileId);

    QSqlDatabase db = QSqlDatabase::database(MICRO_READ_CONN_NAME);
    if (!db.isOpen()) db.open();
    reader_ = new QSqlQuery(db);
    reader_->setForwardOnly(true);

    db = QSqlDatabase::database(MICRO_WRITE_CONN_NAME);
    if (!db.isOpen()) db.open();
    inserter_ = new QSqlQuery(db);

    isDriverExisted_ = false;
    isBusExisted_ = false;
    isPedestrianExisted_ = false;
}

MicroscopicDataDB::~MicroscopicDataDB() {
    if (inserter_) {
        QSqlDatabase::database(MICRO_READ_CONN_NAME).close();
        inserter_->clear();
        delete inserter_;
    }
    if (reader_) {
        QSqlDatabase::database(MICRO_WRITE_CONN_NAME).close();
        reader_->clear();
        delete reader_;
    }
}

void MicroscopicDataDB::insert(CrossingPhaseData *crossingPhaseData) {

}

void MicroscopicDataDB::insert(TrafficPhaseData* trafficPhaseData) {

}

void MicroscopicDataDB::insert(Agent& data) {
    countAgentRecords_++;
    switch (data.getType()) {
        case iSimGUI::AGENT_DRIVER:
            driverBuffer_ << data.sqlInsertValue();
            break;
        case iSimGUI::AGENT_BUS:
            busBuffer_ << data.sqlInsertValue();
            break;
        case iSimGUI::AGENT_PEDESTRIAN:
            pedestrianBuffer_ << data.sqlInsertValue();
            break;
        default: break;
    }

    if (countAgentRecords_ > MAX_BUFFER_SIZE) {
        insertAgentsToDB();
        countAgentRecords_ = 0;
    }
}

void MicroscopicDataDB::updateCrossingPhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {

}

AgentList* MicroscopicDataDB::getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    // remove old data
    AgentList *agentList = new AgentList();
    agentList->setDeepDelete();
    QList<Agent*>& list = agentList->getList();

    // load driver
    if (isDriverExisted_) {
        QString whereQuery = "frame = " + QString::number(tick) + " AND "
                             "xpos BETWEEN " + QString::number(bottomLeft.x()) + " AND " + QString::number(topRight.x()) + " AND "
                             "ypos BETWEEN " + QString::number(bottomLeft.y()) + " AND " + QString::number(topRight.y());

        QString driverQuery = "SELECT * FROM \"" + fileId_ + DBManager::driverTableName + "\" WHERE " + whereQuery;
        reader_->exec(driverQuery);
        unsigned long id, currentSegment;
        double xpos, ypos, angle, length, width;
        int mandatory, fwdSpeed, fwdAccel;
        QString info;
        while (reader_->next()) {
            id = reader_->value(0).toULongLong();
            xpos = reader_->value(2).toDouble();
            ypos = -(reader_->value(3).toDouble());
            angle = reader_->value(4).toDouble();
            length = reader_->value(5).toDouble();
            width = reader_->value(6).toDouble();
            mandatory = reader_->value(7).toInt();
            currentSegment = reader_->value(8).toULongLong();
            fwdSpeed = reader_->value(9).toInt();
            fwdAccel = reader_->value(10).toInt();
            info = reader_->value(11).toString();
            Agent *agent = new Driver(id, tick, QPointF(xpos, ypos), angle, length, width, currentSegment, fwdSpeed, fwdAccel, mandatory, info);
            list.append(agent);
        }
    }


    // load bus
    if (isBusExisted_) {
        QString whereQuery = "frame = " + QString::number(tick) + " AND "
                             "xpos BETWEEN " + QString::number(bottomLeft.x()) + " AND " + QString::number(topRight.x()) + " AND "
                             "ypos BETWEEN " + QString::number(bottomLeft.y()) + " AND " + QString::number(topRight.y());

        QString busQuery = "SELECT * FROM \"" + fileId_ + DBManager::busTableName + "\" WHERE " + whereQuery;
        reader_->exec(busQuery);
        unsigned long id, realArrivalTime;
        double xpos, ypos, angle, length, width;
        int passenger, dwellTime;
        QString busLineID;
        while (reader_->next()) {
            id = reader_->value(0).toULongLong();
            // frame is ignore
            xpos = reader_->value(2).toDouble();
            ypos = -(reader_->value(3).toDouble());
            angle = reader_->value(4).toDouble();
            length = reader_->value(5).toDouble();
            width = reader_->value(6).toDouble();
            passenger = reader_->value(7).toInt();
            realArrivalTime = reader_->value(8).toULongLong();
            dwellTime = reader_->value(9).toInt();
            busLineID = reader_->value(10).toString();
            Agent *agent = new BusDriver(id, tick, QPointF(xpos, ypos), angle, length, width, passenger, realArrivalTime, dwellTime, busLineID);
            list.append(agent);
        }
    }

    // load pedstrian
    if (isPedestrianExisted_) {
        QString whereQuery = "frame = " + QString::number(tick) + " AND "
                             "xpos BETWEEN " + QString::number(bottomLeft.x()) + " AND " + QString::number(topRight.x()) + " AND "
                             "ypos BETWEEN " + QString::number(bottomLeft.y()) + " AND " + QString::number(topRight.y());

        QString pedestrianQuery = "SELECT * FROM \"" + fileId_ + DBManager::pedestrianTableName + "\" WHERE " + whereQuery;
        reader_->exec(pedestrianQuery);
        unsigned long id;
        double xpos, ypos;
        while (reader_->next()) {
            id = reader_->value(0).toULongLong();
            // frame is ignore
            xpos = reader_->value(2).toDouble();
            ypos = -(reader_->value(3).toDouble());
            //angle = reader_->value(4).toDouble();
            Agent *agent = new Pedestrian(id, tick, QPointF(xpos, ypos));
            list.append(agent);
        }
    }

    reader_->clear();

    return agentList;
}

int MicroscopicDataDB::getCrossingPhaseColor(unsigned int tick, unsigned long crossingId) {
    return 0;
}

TrafficPhaseData* MicroscopicDataDB::getTrafficPhaseData(unsigned int tick, unsigned long id) {
    return 0;
}

void MicroscopicDataDB::finishInsertingData() {
    insertAgentsToDB();
}

void MicroscopicDataDB::insertAgentsToDB() {
    if (driverBuffer_.size() > 0) {
        isDriverExisted_ = true;
        QString insertDriverQuery =
                "INSERT INTO \"" + fileId_ + DBManager::driverTableName + "\""
                "(id, frame, xpos, ypos, angle, length, width, mandatory, currentSegment, fwdSpeed, fwdAccel, info)"
                "VALUES " + driverBuffer_.join(",");
        inserter_->exec(insertDriverQuery);
        driverBuffer_.clear();
    }

    if (busBuffer_.size() > 0) {
        isBusExisted_ = true;
        QString insertBusQuery =
                "INSERT INTO \"" + fileId_ + DBManager::busTableName + "\""
                "(id, frame, xpos, ypos, angle, length, width, passenger, realArrivalTime, dwellTime, busLineID)"
                "VALUES " + busBuffer_.join(",");
        inserter_->exec(insertBusQuery);
        busBuffer_.clear();
    }

    if (pedestrianBuffer_.size() > 0) {
        isPedestrianExisted_ = true;
        QString insertPedestrianQuery =
                "INSERT INTO \"" + fileId_ + DBManager::pedestrianTableName + "\""
                "(id, frame, xpos, ypos, angle)"
                "VALUES " + pedestrianBuffer_.join(",");
        inserter_->exec(insertPedestrianQuery);
        pedestrianBuffer_.clear();
    }
}
