#include "microscopicdatadb.h"
#define MAX_BUFFER_SIZE 5000

MicroscopicDataDB::MicroscopicDataDB(int fileId)
    : reader_(QSqlDatabase::database(MICRO_READ_CONN_NAME)){
    fileId_ = QString::number(fileId);

    isAgentExisted_ = false;
    isPhaseDataExisted_ = false;
    QString agentInsertQuery =
            "INSERT INTO \"" + fileId_ + DBManager::agentTableName + "\""
            "(id, frame, xpos, ypos, angle, typeid, attributes)"
            "VALUES ";
    agentInsertWorker_ = new DBInserter(QSqlDatabase::database(MICRO_AGENT_WRITE_CONN_NAME), agentInsertQuery);
    QString phaseInsertQuery =
            "INSERT INTO \"" + fileId_ + DBManager::phaseDataTableName + "\""
            "(typeid, id, frame, nameid, color, nodeid)"
            "VALUES ";
    phaseInsertWorker_ = new DBInserter(QSqlDatabase::database(MICRO_PHASE_WRITE_CONN_NAME), phaseInsertQuery);
}

MicroscopicDataDB::~MicroscopicDataDB() {
    agentInsertWorker_->wait();
    phaseInsertWorker_->wait();

    delete agentInsertWorker_;
    delete phaseInsertWorker_;
}

void MicroscopicDataDB::insert(CrossingPhaseData& crossingPhaseData) {
    phaseInsertWorker_->insertBuffer(crossingPhaseData.sqlInsertValue());

    if ((!phaseInsertWorker_->isRunning() && phaseInsertWorker_->bufferSize() > MAX_BUFFER_SIZE)
        ||
        (phaseInsertWorker_->isRunning() && phaseInsertWorker_->bufferSize() > 4*MAX_BUFFER_SIZE)) {
        insertPhaseToDB();
    }
}

void MicroscopicDataDB::insert(TrafficPhaseData& trafficPhaseData) {
    phaseInsertWorker_->insertBuffer(trafficPhaseData.sqlInsertValue());

    if ((!phaseInsertWorker_->isRunning() && phaseInsertWorker_->bufferSize() > MAX_BUFFER_SIZE)
        ||
        (phaseInsertWorker_->isRunning() && phaseInsertWorker_->bufferSize() > 4*MAX_BUFFER_SIZE)){
        insertPhaseToDB();
    }
}

void MicroscopicDataDB::insert(Agent& data) {
    agentInsertWorker_->insertBuffer(data.sqlInsertValue());

    if ((!agentInsertWorker_->isRunning() && agentInsertWorker_->bufferSize() > MAX_BUFFER_SIZE)
        ||
        (agentInsertWorker_->isRunning() && agentInsertWorker_->bufferSize() > 4*MAX_BUFFER_SIZE)){
        insertAgentsToDB();
    }
}

void MicroscopicDataDB::updatePhaseData(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    if (!isPhaseDataExisted_) return;

    QPoint extendedRange(5000, 5000);
    QPoint newBottom = bottomLeft - extendedRange;
    QPoint newTop = topRight + extendedRange;

    QString phaseTable = "\"" + fileId_ + DBManager::phaseDataTableName + "\"";
    QString nodeTable = "\"" + fileId_ + DBManager::nodeTableName + "\"";

    QString whereClause = "WHERE phase.frame IN ( SELECT MAX(frame) FROM " + phaseTable + " WHERE frame <= " + QString::number(tick) + ") "
                          "AND node.xpos BETWEEN " + QString::number(newBottom.x()) + " AND " + QString::number(newTop.x()) + " "
                          "AND node.ypos BETWEEN " + QString::number(newBottom.y()) + " AND " + QString::number(newTop.y());
    QString phaseQuery = "SELECT phase.typeid, phase.id, phase.frame, phase.nodeid, phase.color, phase.nameid FROM " + phaseTable +" AS phase "
                         "INNER JOIN " + nodeTable + " AS node ON (node.id = phase.nodeid) " + whereClause;

    reader_.setForwardOnly(true);
    if (!reader_.exec(phaseQuery))
        qDebug() << reader_.lastError().text();

    QHash<unsigned long, CrossingPhaseData> crossingPhaseData;
    QHash<unsigned long, TrafficPhaseData> trafficPhaseData;

    unsigned long id, nodeid;
    int type, frame, color;
    QString name, colors;
    while (reader_.next()) {
        type = reader_.value(0).toInt();
        id = reader_.value(1).toULongLong();
        frame = reader_.value(2).toInt();
        nodeid = reader_.value(3).toInt();
        if (type == 1) { // crossing
            color = reader_.value(4).toInt();
            crossingPhaseData[id] = CrossingPhaseData(frame, id, color, nodeid);
        } else {
            colors = reader_.value(4).toString();
            name = reader_.value(5).toString();
            TrafficPhaseData temp(id, frame, name, nodeid);
            QStringList listColors = colors.split(",");
            for (int i = 0; i < listColors.size(); i++)
                temp.colors.append(listColors.at(i).toInt());
            trafficPhaseData[id] = temp;
        }
    }

    crossingPhaseMutex_.lock();
    crossingPhaseData_ = crossingPhaseData;
    crossingPhaseMutex_.unlock();

    trafficPhaseMutex_.lock();
    trafficPhaseData_ = trafficPhaseData;
    trafficPhaseMutex_.unlock();

    reader_.clear();
}

AgentList* MicroscopicDataDB::getAgent(unsigned int tick, QPoint& bottomLeft, QPoint& topRight) {
    // remove old data
    AgentList *agentList = new AgentList();
    agentList->setDeepDelete();
    QList<Agent*>& list = agentList->getList();

    // load driver
    if (isAgentExisted_) {
        QString whereQuery = "frame = " + QString::number(tick) + " AND "
                             "xpos BETWEEN " + QString::number(bottomLeft.x()) + " AND " + QString::number(topRight.x()) + " AND "
                             "ypos BETWEEN " + QString::number(bottomLeft.y()) + " AND " + QString::number(topRight.y());
        QString driverQuery = "SELECT * FROM \"" + fileId_ + DBManager::agentTableName + "\" WHERE " + whereQuery;

        reader_.setForwardOnly(true);
        reader_.exec(driverQuery);

        unsigned long id;
        double xpos, ypos, angle;
        int type;
        QStringList attributes;
        while (reader_.next()) {
            id = reader_.value(0).toULongLong();
            xpos = reader_.value(2).toDouble();
            ypos = -(reader_.value(3).toDouble());
            angle = reader_.value(4).toDouble();
            type = reader_.value(5).toInt();
            attributes = (reader_.value(6).toString()).split(":");

            Agent *agent = 0;
            if (type == 0) { // driver
                double length = attributes.at(0).toDouble();
                double width = attributes.at(1).toDouble();
                int mandatory = attributes.at(2).toInt();
                unsigned long currentSegment = attributes.at(3).toULongLong();
                int fwdSpeed = attributes.at(4).toInt();
                int fwdAccel = attributes.at(5).toInt();
                agent = new Driver(id, tick, QPointF(xpos, ypos), angle, length, width, currentSegment, fwdSpeed, fwdAccel, mandatory, attributes.at(6));
            } else if (type == 1) { // bus
                double length = attributes.at(0).toDouble();
                double width = attributes.at(1).toDouble();
                int passenger = attributes.at(2).toInt();
                unsigned long realArrivalTime = attributes.at(3).toULongLong();
                int dwellTime = attributes.at(4).toInt();
                agent = new BusDriver(id, tick, QPointF(xpos, ypos), angle, length, width, passenger, realArrivalTime, dwellTime, attributes.at(5));
            } else {
                agent = new Pedestrian(id, tick, QPointF(xpos, ypos));
            }

            list.append(agent);
        }
        reader_.clear();
    }
    return agentList;
}

int MicroscopicDataDB::getCrossingPhaseColor(unsigned int tick, unsigned long crossingId) {
    Q_UNUSED(tick)
    int color = 0;

    crossingPhaseMutex_.lock();
    if (crossingPhaseData_.contains(crossingId))
        color = crossingPhaseData_[crossingId].color;
    crossingPhaseMutex_.unlock();

    return color;
}

TrafficPhaseData MicroscopicDataDB::getTrafficPhaseData(unsigned int tick, unsigned long id) {
    Q_UNUSED(tick)
    TrafficPhaseData temp;

    trafficPhaseMutex_.lock();
    if (trafficPhaseData_.contains(id))
        temp = trafficPhaseData_[id];
    trafficPhaseMutex_.unlock();

    return temp;
}

void MicroscopicDataDB::finishInsertingData() {
    agentInsertWorker_->wait();
    if (agentInsertWorker_->exeQuery() > 0) isAgentExisted_ = true;
    phaseInsertWorker_->wait();
    if (phaseInsertWorker_->exeQuery() > 0) isPhaseDataExisted_ = true;
}

void MicroscopicDataDB::insertAgentsToDB() {
    if (agentInsertWorker_->bufferSize() > 0) {
        if (agentInsertWorker_->isRunning())
            agentInsertWorker_->wait();
        agentInsertWorker_->start();
        isAgentExisted_ = true;
    }
}

void MicroscopicDataDB::insertPhaseToDB() {
    if (phaseInsertWorker_->bufferSize() > 0) {
        if (phaseInsertWorker_->isRunning())
            phaseInsertWorker_->wait();
        phaseInsertWorker_->start();
        isPhaseDataExisted_ = true;
    }
}

void MicroscopicDataDB::setDataExisted() {
    isAgentExisted_ = true;
    isPhaseDataExisted_ = true;
}
