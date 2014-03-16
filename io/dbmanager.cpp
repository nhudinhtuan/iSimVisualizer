#include "dbmanager.h"

const QString DBManager::recordTableName = "isimguirecords";
const QString DBManager::agentTableName = "_isimgui_agents";
const QString DBManager::nodeTableName = "_isimgui_nodes";
const QString DBManager::phaseDataTableName = "_isimgui_phases";

DBManager::DBManager(PreferenceManager *preferenceManager)
    : preferenceManager_(preferenceManager){
    handleDb_ = QSqlDatabase::addDatabase("QPSQL", HANDLER_CONN_NAME);
    currentFileId_ = 0;
}

DBManager::~DBManager() {
    close();
    handleDb_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(HANDLER_CONN_NAME);
    QSqlDatabase::database(GEO_WRITER_CONN_NAME).close();
    QSqlDatabase::removeDatabase(GEO_WRITER_CONN_NAME);
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

    if (QSqlDatabase::contains(GEO_WRITER_CONN_NAME)) {
        QSqlDatabase::database(GEO_WRITER_CONN_NAME).close();
        QSqlDatabase::removeDatabase(GEO_WRITER_CONN_NAME);
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
    QSqlDatabase::cloneDatabase(handleDb_, GEO_WRITER_CONN_NAME);
    QSqlDatabase::database(GEO_WRITER_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_READ_CONN_NAME);
    QSqlDatabase::database(MICRO_READ_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_AGENT_WRITE_CONN_NAME);
    QSqlDatabase::database(MICRO_AGENT_WRITE_CONN_NAME).open();
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_PHASE_WRITE_CONN_NAME);
    QSqlDatabase::database(MICRO_PHASE_WRITE_CONN_NAME).open();
}

bool DBManager::open() {
    if (handleDb_.isOpen())
        close();
    return handleDb_.open();
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
    query.addBindValue(size);
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

void DBManager::markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted) {
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
    query.prepare("UPDATE \"" + recordTableName + "\" SET iscompleted=true, "
                  "ismicroexisted=:ismic, ismesoexisted=:ismes, ismacroexisted=:ismac WHERE id=:id");
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
        query.exec("SELECT id FROM \"" + recordTableName + "\" WHERE iscompleted = false");
        while (query.next()) {
            QString id = query.value(0).toString();
            removeIds << id;
            removeTables << "\"" + id + agentTableName + "\"";
            removeTables << "\"" + id + phaseDataTableName + "\"";
            removeTables << "\"" + id + nodeTableName + "\"";
        }
        if (removeTables.size() > 0)
            query.exec("DROP TABLE IF EXISTS " + removeTables.join(","));
        if (removeIds.size() > 0)
            query.exec("DELETE FROM \"" + recordTableName + "\" WHERE id IN (" + removeIds.join(",") + ")");
        query.clear();
    }
    return true;
}

bool DBManager::createRecordTable() {
    if (!handleDb_.isOpen()) return false;
    QSqlQuery query(handleDb_);
    QString tableSchema = "CREATE TABLE \"" + recordTableName + "\" ("
        "id serial primary key, "
        "path varchar(100), "
        "size integer, "
        "iscompleted boolean DEFAULT false, "
        "ismicroexisted boolean DEFAULT false, "
        "ismesoexisted boolean DEFAULT false, "
        "ismacroexisted boolean DEFAULT false)";
    bool result = query.exec(tableSchema);
    query.clear();
    return result;
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
            "aimsunId bigint,"
            "xpos double precision,"
            "ypos double precision)";
    result = result & query.exec(nodeSchema);

    // create

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
