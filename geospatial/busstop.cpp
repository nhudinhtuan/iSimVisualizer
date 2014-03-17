#include "busstop.h"

const int BusStop::DB_ID = 4;

BusStop::BusStop(unsigned long id, unsigned long aimsunId, QPointF pos)
{
    id_ = id;
    aimsunId_ = aimsunId;
    pos_ = pos;
}

BusStop::BusStop(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    aimsunId_ = attributes[0].toULongLong();
    QStringList posStr = attributes[1].split(":");
    pos_.setX(posStr[0].toDouble());
    pos_.setY(-(posStr[1].toDouble()));
}

QString BusStop::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList info;
    info << QString::number(aimsunId_);
    info << QString::number(pos_.x(), 'f', 2) + ":" + QString::number(-pos_.y(), 'f', 2);

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}
