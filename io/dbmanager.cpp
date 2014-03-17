#include "dbmanager.h"

const QString DBManager::recordTableName = "isimguirecords";
const QString DBManager::agentTableName = "_isimgui_agents";
const QString DBManager::nodeTableName = "_isimgui_nodes";
const QString DBManager::geoTableName = "_isimgui_geos";
const QString DBManager::phaseDataTableName = "_isimgui_phases";

DBManager::DBManager(QObject *parent, PreferenceManager *preferenceManager)
    : QObject(parent), preferenceManager_(preferenceManager){
    handleDb_ = QSqlDatabase::addDatabase("QPSQL", HANDLER_CONN_NAME);
    currentFileId_ = 0;
}

DBManager::~DBManager() {
    close();
    handleDb_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(HANDLER_CONN_NAME);
    QSqlDatabase::database(GEO_CONN_NAME).close();
    QSqlDatabase::removeDatabase(GEO_CONN_NAME);
    QSqlDatabase::database(MICRO_READ_CONN_NAME).close();
    QSqlDatabase::removeDatabase(MICRO_READ_CONN_NAME);
    QSqlDatabase::database(MICRO_AGENT_WRITE_CONN_NAME).close();
    QSqlDatabase::removeDatabase(MICRO_AGENT_WRITE_CONN_NAME);
    QSqlDatabase::database(MICRO_PHASE_WRITE_CONN_NAME).close();
    QSqlDatabase::removeDatabase(MICRO_PHASE_WRITE_CONN_NAME);
}

void DBManager::config() {
    close();
    handleDb_.setHostName(preferenceManager_->getDBHost());
    handleDb_.setPort(preferenceManager_->getDBPort());
    handleDb_.setUserName(preferenceManager_->getDBUsername());
    handleDb_.setPassword(preferenceManager_->getDBPass());
    handleDb_.setDatabaseName(preferenceManager_->getDBName());

    if (QSqlDatabase::contains(GEO_CONN_NAME)) {
        QSqlDatabase::database(GEO_CONN_NAME).close();
        QSqlDatabase::removeDatabase(GEO_CONN_NAME);
    }
    if (QSqlDatabase::contains(MICRO_READ_CONN_NAME)) {
        QSqlDatabase::database(MICRO_READ_CONN_NAME).close();
        QSqlDatabase::removeDatabase(MICRO_READ_CONN_NAME);
    }
    if (QSqlDatabase::contains(MICRO_AGENT_WRITE_CONN_NAME)) {
        QSqlDatabase::database(MICRO_AGENT_WRITE_CONN_NAME).close();
        QSqlDatabase::removeDatabase(MICRO_AGENT_WRITE_CONN_NAME);
    }
    if (QSqlDatabase::contains(MICRO_PHASE_WRITE_CONN_NAME)) {
        QSqlDatabase::database(MICRO_PHASE_WRITE_CONN_NAME).close();
        QSqlDatabase::removeDatabase(MICRO_PHASE_WRITE_CONN_NAME);
    }
    QSqlDatabase::cloneDatabase(handleDb_, GEO_CONN_NAME);
    //QSqlDatabase::database(GEO_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_READ_CONN_NAME);
    //QSqlDatabase::database(MICRO_READ_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_AGENT_WRITE_CONN_NAME);
    //QSqlDatabase::database(MICRO_AGENT_WRITE_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_PHASE_WRITE_CONN_NAME);
    //QSqlDatabase::database(MICRO_PHASE_WRITE_CONN_NAME).open();
}

bool DBManager::open() {
    if (!handleDb_.isOpen())
        return handleDb_.open();
    return true;
}

void DBManager::close() {
    currentFileId_ = 0;
    if (handleDb_.isOpen())
        handleDb_.close();
}

int DBManager::prepareTables(QString path) {
    if (!handleDb_.isOpen()) return 0;
    if (!prepareDB()) return 0;

    currentFileId_ = 0;
    QFileInfo fileInfo(path);
    qint64 size = fileInfo.size();
    QSqlQuery query(handleDb_);
    query.prepare("INSERT INTO \"" + recordTableName + "\" (path, size) "
               "VALUES (?, ?)");
    query.addBindValue(path);
    query.addBindValue(size/1024); //kb
    if (query.exec()) {
        currentFileId_ = query.lastInsertId().toInt();
    } else {
        qDebug() << query.lastError();
    }
    query.clear();

    bool result = true;
    result = result & createGeoTables(currentFileId_);
    result = result & createMicroTables(currentFileId_);

    if (result)
        return currentFileId_;
    return 0;
}

void DBManager::markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted, int nframe) {
    if (!handleDb_.isOpen() || !currentFileId_) return;

    // create index
    QSqlQuery query(handleDb_);
    QString id = QString::number(currentFileId_);
    query.exec("CREATE INDEX \"" + id + agentTableName + "_id\" ON \"" + id + agentTableName + "\" (frame, xpos, ypos)");
    query.exec("CREATE INDEX \"" + id + phaseDataTableName + "_id\" ON \"" + id + phaseDataTableName + "\" (frame, nodeId)");
    query.exec("CREATE INDEX \"" + id + nodeTableName + "_id\" ON \"" + id + nodeTableName + "\" (id, xpos, ypos)");

    QString isMacroExistedStr = isMacroExisted ? "true" : "false";
    QString isMesoExistedStr = isMesoExisted ? "true" : "false";
    QString isMicroExistedStr = isMicroExisted ? "true" : "false";
    query.prepare("UPDATE \"" + recordTableName + "\" SET iscompleted=true, nframe=:nframe, "
                  "ismicroexisted=:ismic, ismesoexisted=:ismes, ismacroexisted=:ismac WHERE id=:id");
    query.bindValue(":nframe", nframe);
    query.bindValue(":ismic", isMicroExistedStr);
    query.bindValue(":ismes", isMesoExistedStr);
    query.bindValue(":ismac", isMacroExistedStr);
    query.bindValue(":id", currentFileId_);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
    query.clear();
}

bool DBManager::prepareDB() {
    if (!handleDb_.isOpen()) return false;
    QStringList tables = handleDb_.tables();
    if (!tables.contains(recordTableName)) {
        return createRecordTable();
    } else {
        // remove redundant
        QSqlQuery query(handleDb_);
        QStringList removeTables;
        QStringList removeIds;
        query.setForwardOnly(true);
        query.exec("SELECT id FROM \"" + recordTableName + "\" WHERE iscompleted = false");
        while (query.next()) {
            QString id = query.value(0).toString();
            removeIds << id;
            removeTables << "\"" + id + agentTableName + "\"";
            removeTables << "\"" + id + phaseDataTableName + "\"";
            removeTables << "\"" + id + nodeTableName + "\"";
            removeTables << "\"" + id + geoTableName + "\"";
        }
        if (removeTables.size() > 0)
            query.exec("DROP TABLE IF EXISTS " + removeTables.join(","));
        if (removeIds.size() > 0)
            query.exec("DELETE FROM \"" + recordTableName + "\" WHERE id IN (" + removeIds.join(",") + ")");
        query.clear();
    }
    return true;
}

bool DBManager::removeRecord(int id) {
    if (currentFileId_ == id)
        emit requestResetWorkspace();

    QString idStr = QString::number(id);
    QStringList removeTables;
    removeTables << "\"" + idStr + agentTableName + "\"";
    removeTables << "\"" + idStr + phaseDataTableName + "\"";
    removeTables << "\"" + idStr + nodeTableName + "\"";
    removeTables << "\"" + idStr + geoTableName + "\"";
    QSqlQuery query(handleDb_);
    bool result = true;

    result = query.exec("DROP TABLE IF EXISTS " + removeTables.join(","));
    result = result & query.exec("DELETE FROM \"" + recordTableName + "\" WHERE id =" + idStr + "");
    return result;
}

bool DBManager::createRecordTable() {
    if (!handleDb_.isOpen()) return false;
    QSqlQuery query(handleDb_);
    QString tableSchema = "CREATE TABLE \"" + recordTableName + "\" ("
        "id serial primary key, "
        "path varchar(100), "
        "size integer, "
        "nframe integer DEFAULT 0, "
        "iscompleted boolean DEFAULT false, "
        "ismicroexisted boolean DEFAULT false, "
        "ismesoexisted boolean DEFAULT false, "
        "ismacroexisted boolean DEFAULT false, "
        "created timestamp DEFAULT CURRENT_TIMESTAMP)";
    bool result = query.exec(tableSchema);
    query.clear();
    return result;
}

QList<Record> DBManager::getAllRecords() {
    QList<Record> list;
    if (!handleDb_.isOpen()) return list;

    QStringList tables = handleDb_.tables();
    if (!tables.contains(recordTableName)) return list;

    QSqlQuery query(handleDb_);
    query.setForwardOnly(true);
    query.exec("SELECT id, path, size, nframe, ismicroexisted, ismesoexisted, ismacroexisted, created FROM \"" + recordTableName + "\" WHERE iscompleted = true");
    while (query.next()) {
        Record record;
        record.id = query.value(0).toInt();
        record.path = query.value(1).toString();
        record.size = query.value(2).toInt();
        record.nframe = query.value(3).toInt();
        record.isMicro = query.value(4).toBool();
        record.isMesos = query.value(5).toBool();
        record.isMacro = query.value(6).toBool();
        record.created = query.value(7).toString();
        list.append(record);
    }
    query.clear();
    return list;
}

Record DBManager::getRecord(int id) {
    Record record;
    if (!handleDb_.isOpen()) return record;

    QSqlQuery query(handleDb_);
    query.setForwardOnly(true);
    query.exec("SELECT id, path, size, nframe, ismicroexisted, ismesoexisted, ismacroexisted, created FROM \"" + recordTableName + "\" WHERE id = " + QString::number(id));
    if (query.next()) {
        record.id = query.value(0).toInt();
        record.path = query.value(1).toString();
        record.size = query.value(2).toInt();
        record.nframe = query.value(3).toInt();
        record.isMicro = query.value(4).toBool();
        record.isMesos = query.value(5).toBool();
        record.isMacro = query.value(6).toBool();
        record.created = query.value(7).toString();
    }
    query.clear();
    return record;
}

bool DBManager::createMicroTables(int fileId) {
    if (!handleDb_.isOpen()) return false;

    QSqlQuery query(handleDb_);
    QString fileIdStr = QString::number(fileId);

    bool result =  true;

    // create agent table
    QString agentSchema = "CREATE TABLE \"" + fileIdStr + agentTableName + "\" ("
        "id bigint NOT NULL, "
        "frame integer NOT NULL, "
        "xpos double precision, "
        "ypos double precision, "
        "angle double precision, "
        "typeid varchar(1),"
        "attributes text)";
    result = result & query.exec(agentSchema);

    // create phase data table
    // type can be crossingPhase(1) or trafficPhase(2)
    QString phaseDataSchema = "CREATE TABLE \"" + fileIdStr + phaseDataTableName + "\" ("
        "typeid varchar(1),"
        "id bigint NOT NULL, "
        "frame integer NOT NULL, "
        "nameid varchar(10), "
        "color varchar(30),"
        "nodeid bigint)";
    result = result & query.exec(phaseDataSchema);

    query.clear();
    return result;
}

bool DBManager::createGeoTables(int fileId) {
    if (!handleDb_.isOpen()) return false;
    QSqlQuery query(handleDb_);

    QString fileIdStr = QString::number(fileId);
    bool result = true;
    // create node table
    QString nodeSchema = "CREATE TABLE \"" + fileIdStr + nodeTableName +"\" ("
            "id bigint NOT NULL, "
            "typeid varchar(1),"
            "aimsunid bigint,"
            "xpos double precision,"
            "ypos double precision)";
    result = result & query.exec(nodeSchema);

    // create
    QString geoSchema = "CREATE TABLE \"" + fileIdStr + geoTableName +"\" ("
            "id bigint NOT NULL, "
            "typeid varchar(1),"
            "info text)";
    result = result & query.exec(geoSchema);

    query.clear();
    return result;
}

bool DBManager::testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr) {

    bool isSuccess = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", TEST_CONN_NAME);
        db.setHostName(host);
        db.setPort(port);
        db.setUserName(username);
        db.setPassword(password);
        db.setDatabaseName(dbName);
        isSuccess = db.open();
        if (!isSuccess) {
            QSqlError error = db.lastError();
            *errorStr = error.text();
        }
        db.close();
    }
    QSqlDatabase::removeDatabase(TEST_CONN_NAME);
    return isSuccess;
}
