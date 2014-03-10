#include "dbmanager.h"

const QString DBManager::recordTableName = "isimguirecords";
const QString DBManager::driverTableName = "_isimgui_driver";
const QString DBManager::busTableName = "_isimgui_bus";
const QString DBManager::pedestrianTableName = "_isimgui_pedestrian";

DBManager::DBManager(PreferenceManager *preferenceManager)
    : preferenceManager_(preferenceManager){
    handleDb_ = QSqlDatabase::addDatabase("QPSQL", HANDLER_CONN_NAME);
    currentFileId_ = 0;
}

DBManager::~DBManager() {
    close();
    handleDb_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(HANDLER_CONN_NAME);
    QSqlDatabase::removeDatabase(MICRO_READ_CONN_NAME);
    QSqlDatabase::removeDatabase(MICRO_WRITE_CONN_NAME);
}

void DBManager::config() {
    close();
    handleDb_.setHostName(preferenceManager_->getDBHost());
    handleDb_.setPort(preferenceManager_->getDBPort());
    handleDb_.setUserName(preferenceManager_->getDBUsername());
    handleDb_.setPassword(preferenceManager_->getDBPass());
    handleDb_.setDatabaseName(preferenceManager_->getDBName());

    if (QSqlDatabase::contains(MICRO_READ_CONN_NAME)) {
        QSqlDatabase::removeDatabase(MICRO_READ_CONN_NAME);
    }
    if (QSqlDatabase::contains(MICRO_WRITE_CONN_NAME)) {
        QSqlDatabase::removeDatabase(MICRO_WRITE_CONN_NAME);
    }
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_READ_CONN_NAME);
    QSqlDatabase::cloneDatabase(handleDb_, MICRO_WRITE_CONN_NAME);
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

int DBManager::prepareTables(QString path, int type) {
    if (!handleDb_.isOpen()) return 0;
    if (!prepareDB()) return 0;

    currentFileId_ = 0;
    QFileInfo fileInfo(path);
    qint64 size = fileInfo.size();
    QSqlQuery query(handleDb_);
    query.prepare("INSERT INTO \"" + recordTableName + "\" (path, type, size) "
               "VALUES (?, ?, ?)");
    query.addBindValue(path);
    query.addBindValue(type);
    query.addBindValue(size);
    if (query.exec()) {
        currentFileId_ = query.lastInsertId().toInt();
    } else {
        qDebug() << query.lastError();
    }
    query.clear();

    if (createMicroTables(currentFileId_))
        return currentFileId_;
    return 0;
}

void DBManager::markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted) {
    if (!handleDb_.isOpen() || !currentFileId_) return;

    // create index
    QSqlQuery query(handleDb_);
    QString id = QString::number(currentFileId_);
    query.exec("CREATE INDEX \"" + id + driverTableName + "_id\" ON \"" + id + driverTableName + "\" (frame, xpos, ypos)");
    query.exec("CREATE INDEX \"" + id + busTableName + "_id\" ON \"" + id + busTableName + "\" (frame, xpos, ypos)");
    query.exec("CREATE INDEX \"" + id + pedestrianTableName + "_id\" ON \"" + id + pedestrianTableName + "\" (frame, xpos, ypos)");


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
        query.exec("SELECT id FROM \"" + recordTableName + "\" WHERE type = '1' OR iscompleted = false");
        while (query.next()) {
            QString id = query.value(0).toString();
            removeIds << id;
            removeTables << "\"" + id + driverTableName + "\"";
            removeTables << "\"" + id + busTableName + "\"";
            removeTables << "\"" + id + pedestrianTableName + "\"";
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
        "type varchar(1), "
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

    bool result = true;
    QSqlQuery query(handleDb_);
    QString fileIdStr = QString::number(fileId);

    // create driver table
    QString driverSchema = "CREATE TABLE \"" + fileIdStr + driverTableName + "\" ("
        "id bigint NOT NULL, "
        "frame integer NOT NULL, "
        "xpos double precision, "
        "ypos double precision, "
        "angle double precision, "
        "length double precision, "
        "width double precision,"
        "mandatory smallint,"
        "currentSegment bigint,"
        "fwdSpeed integer,"
        "fwdAccel integer,"
        "info varchar(10))";
    result = result & query.exec(driverSchema);

    // create driver table
    QString busSchema = "CREATE TABLE \"" + fileIdStr + busTableName + "\" ("
        "id bigint NOT NULL, "
        "frame integer NOT NULL, "
        "xpos double precision, "
        "ypos double precision, "
        "angle double precision, "
        "length double precision, "
        "width double precision,"
        "passenger smallint,"
        "realArrivalTime integer,"
        "dwellTime integer,"
        "busLineID varchar(20))";
    result = result & query.exec(busSchema);

    // create pedestrian table
    QString pedestrianSchema = "CREATE TABLE \"" + fileIdStr + pedestrianTableName + "\" ("
        "id bigint NOT NULL, "
        "frame integer NOT NULL, "
        "xpos double precision, "
        "ypos double precision, "
        "angle double precision)";
    result = result & query.exec(pedestrianSchema);

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
