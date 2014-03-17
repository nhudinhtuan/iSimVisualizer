#include "laneconnector.h"

const int LaneConnector::DB_ID = 3;

LaneConnector::LaneConnector(unsigned long id, unsigned long fromSegment, unsigned int fromLane, unsigned long toSegment, unsigned int toLane)
{
    id_ = id;
    fromSegment_ = fromSegment;
    fromLane_ = fromLane;
    toSegment_ = toSegment;
    toLane_ = toLane;
}

LaneConnector::LaneConnector(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    fromSegment_ = attributes[0].toULongLong();
    fromLane_ = attributes[1].toInt();
    toSegment_ = attributes[2].toULongLong();
    toLane_ = attributes[3].toInt();
}

void LaneConnector::setPoints(QPointF fromPoint, QPointF toPoint) {
    fromPoint_ = fromPoint;
    toPoint_ = toPoint;
}

QString LaneConnector::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList info;
    info << QString::number(fromSegment_);
    info << QString::number(fromLane_);
    info << QString::number(toSegment_);
    info << QString::number(toLane_);

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}
