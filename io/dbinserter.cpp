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

void DBInserter::exeQuery() {
    dataMutex.lock();
    QString completeQuery = preQuery_ + data_.join(",");
    data_.clear();
    dataMutex.unlock();
    if (!sql_.exec(completeQuery))
        qDebug() << sql_.lastError().text();
}
