#ifndef DBINSERTER_H
#define DBINSERTER_H

#include <QThread>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QSqlError>

class DBInserter : public QThread {
    Q_OBJECT

public:
    DBInserter(QSqlDatabase db, QString& preQuery);
    void insertBuffer(QString item);
    int bufferSize();
    ~DBInserter();
    int exeQuery();

protected:
    void run();

private:
    QStringList data_;
    QSqlQuery sql_;
    QString preQuery_;
    QMutex dataMutex;
};

#endif // DBINSERTER_H
