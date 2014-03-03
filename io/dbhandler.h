#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QDebug>

#define HANDLER_CONN_NAME "dbhandler"
#define GEO_CONN_NAME "geodb"
#define TEST_CONN_NAME "testConnection"

class DBHandler
{
public:
    static bool testConnection(QString host, int port, QString username, QString password, QString dbName, QString* errorStr);
    DBHandler();
    ~DBHandler();
    void config(QString host, int port, QString username, QString password, QString dbName);
    bool open();
    void close();
    void cleanUp();

private:
    QSqlDatabase handleDb_;
    QSqlDatabase geoDb_;
    int fileId_;
};

#endif // DBHANDLER_H
