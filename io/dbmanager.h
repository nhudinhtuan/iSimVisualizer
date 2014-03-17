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
#define MICRO_AGENT_WRITE_CONN_NAME "microagentdb"
#define MICRO_PHASE_WRITE_CONN_NAME "microphasedb"
#define TEST_CONN_NAME "testConnection"

struct Record {
    int id, size, nframe;
    bool isMacro, isMesos, isMicro;
    QString path;
    QString created;

    Record() {
        id = size = nframe = 0;
        isMacro = isMesos = isMicro = false;
        path = "";
        created = "";
    }
    Record (const Record& other) {
        id = other.id;
        size = other.size;
        nframe = other.nframe;
        isMacro = other.isMacro;
        isMesos = other.isMesos;
        isMicro = other.isMicro;
        path = other.path;
        created = other.created;
    }
};

class DBManager : public QObject {
    Q_OBJECT

public:
    static const QString recordTableName;
    static const QString agentTableName;
    static const QString nodeTableName;
    static const QString geoTableName;
    static const QString phaseDataTableName;
    static bool testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr);

    explicit DBManager(QObject *parent, PreferenceManager *preferenceManager);
    ~DBManager();
    void config();
    bool open();
    void close();
    int prepareTables(QString path);
    void markFileCompleted(bool isMacroExisted, bool isMesoExisted, bool isMicroExisted, int nframe);
    QList<Record> getAllRecords();
    Record getRecord(int id);
    bool removeRecord(int id);

    int getCurrentFileId() { return currentFileId_;}

signals:
    void requestResetWorkspace();

public slots:

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
