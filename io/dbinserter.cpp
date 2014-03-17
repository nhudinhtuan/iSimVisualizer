#include "dbinserter.h"

DBInserter::DBInserter(QSqlDatabase db, QString& preQuery)
    : sql_(db) {
    if (!db.isOpen()) db.open();
    preQuery_ = preQuery;
}

DBInserter::~DBInserter() {
    sql_.clear();
    sql_.finish();
}

void DBInserter::insertBuffer(QString item) {
    dataMutex.lock();
    data_ << item;
    dataMutex.unlock();
}

int DBInserter::bufferSize() {
    int size;
    dataMutex.lock();
    size = data_.size();
    dataMutex.unlock();
    return size;
}

void DBInserter::run() {
    exeQuery();
}

int DBInserter::exeQuery() {
    int size = 0;
    dataMutex.lock();
    QString completeQuery = preQuery_ + data_.join(",");
    size = data_.size();
    data_.clear();
    dataMutex.unlock();

    if (size == 0) return 0;
    if (!sql_.exec(completeQuery))
        qDebug() << sql_.lastError().text();
    return size;
}
