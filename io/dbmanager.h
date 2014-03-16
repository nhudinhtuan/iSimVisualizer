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
#define GEO_WRITER_CONN_NAME "geodb"
#define MICRO_READ_CONN_NAME "microreaddb"
#define MICRO_AGENT_WRITE_CONN_NAME "microagentdb"
#define MICRO_PHASE_WRITE_CONN_NAME "microphasedb"
#define TEST_CONN_NAME "testConnection"

class DBManager
{
public:
    static const QString recordTableName;
    static const QString agentTableName;
    static const QString nodeTableName;
    static const QString phaseDataTableName;
    static bool testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr);

    DBManager(PreferenceManager *preferenceManager);
    ~DBManager();
    void config();
    bool open();
    void close();
    int prepareTables(QString path);
    void markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted);

private:
    bool prepareDB();
    bool createRecordTable();
    bool createMicroTables(int fileId);
    bool createGeoTables(int fileId);

    PreferenceManager *preferenceManager_;
    QSqlDatabase handleDb_;
    int currentFileId_;
};

#endif // DBManager_H
