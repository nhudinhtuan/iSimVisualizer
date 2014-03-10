#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QString>
#include <QDebug>
#include "preferencemanager.h"

#define HANDLER_CONN_NAME "DBManager"
#define GEO_CONN_NAME "geodb"
#define MICRO_READ_CONN_NAME "microreaddb"
#define MICRO_WRITE_CONN_NAME "microwritedb"
#define TEST_CONN_NAME "testConnection"

class DBManager
{
public:
    static const QString recordTableName;
    static const QString driverTableName;
    static const QString busTableName;
    static const QString pedestrianTableName;
    static bool testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr);

    DBManager(PreferenceManager *preferenceManager);
    ~DBManager();
    void config();
    bool open();
    void close();
    int prepareTables(QString path, int type);
    void markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted);

private:
    bool prepareDB();
    bool createRecordTable();
    bool createMicroTables(int fileId);

    PreferenceManager *preferenceManager_;
    QSqlDatabase handleDb_;
    int currentFileId_;
};

#endif // DBManager_H
