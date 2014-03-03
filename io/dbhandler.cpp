#include "dbhandler.h"

DBHandler::DBHandler() {
    handleDb_ = QSqlDatabase::addDatabase("QPSQL", HANDLER_CONN_NAME);
    geoDb_ = QSqlDatabase::addDatabase("QPSQL", GEO_CONN_NAME);
    fileId_ = 0;
}

DBHandler::~DBHandler() {
    close();
    handleDb_ = QSqlDatabase();
    geoDb_ = QSqlDatabase();
    QSqlDatabase::removeDatabase(HANDLER_CONN_NAME);
    QSqlDatabase::removeDatabase(GEO_CONN_NAME);
}

void DBHandler::config(QString host, int port, QString username, QString password, QString dbName) {
    close();
    handleDb_.setHostName(host);
    handleDb_.setPort(port);
    handleDb_.setUserName(username);
    handleDb_.setPassword(password);
    handleDb_.setDatabaseName(dbName);
    geoDb_.setHostName(host);
    geoDb_.setPort(port);
    geoDb_.setUserName(username);
    geoDb_.setPassword(password);
    geoDb_.setDatabaseName(dbName);
}

bool DBHandler::open() {
    fileId_ = 0;
    if (handleDb_.isOpen())
        return true;
    return handleDb_.open();
}

void DBHandler::close() {
    if (handleDb_.isOpen())
        handleDb_.close();
    if (geoDb_.isOpen())
        geoDb_.close();
    fileId_ = 0;
}

void DBHandler::cleanUp() {

}

bool DBHandler::testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr) {

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
