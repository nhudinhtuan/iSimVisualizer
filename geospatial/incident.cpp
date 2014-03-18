#include "incident.h"

const int Incident::DB_ID = 7;

Incident::Incident(unsigned long id, unsigned long segment_aimsun_id, int posPercentage, int start_time, int duration, QList<int> lane,
                  int visibility, int length) {
    id_ = id;
    segment_aimsun_id_ = segment_aimsun_id;
    start_time_ = start_time;
    duration_ = duration;
    lane_ = lane;
    visibility_ = visibility;
    length_ = length;
    posPercentage_ = posPercentage;
}

Incident::Incident(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    segment_aimsun_id_ = attributes[0].toULongLong();
    start_time_ = attributes[1].toInt();
    duration_ = attributes[2].toInt();
    visibility_ = attributes[3].toInt();
    length_ = attributes[4].toInt();
    posPercentage_ = attributes[5].toInt();

    QStringList laneIdStr = attributes[6].split(",");
    for (int i = 0; i < laneIdStr.size(); i++)
        lane_.append(laneIdStr[i].toInt());
}

QString Incident::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList info;
    info << QString::number(segment_aimsun_id_);
    info << QString::number(start_time_);
    info << QString::number(duration_);
    info << QString::number(visibility_);
    info << QString::number(length_);
    info << QString::number(posPercentage_);

    QStringList laneIdStr;
    for (int i = 0; i < lane_.size(); i++)
        laneIdStr << QString::number(lane_[i]);
    info << laneIdStr.join(",");

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}

QPair<QPointF, QPointF> Incident::getPosFromLane(qreal x1, qreal y1, qreal x2, qreal y2) {
    QPointF pos1, pos2;
    qreal laneLength = qSqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
    qreal lengthFactor = length_/laneLength;

    // calculate the position
    if (x1 > x2) {
        pos1.setX(x1 - ((x1 - x2)*posPercentage_)/100);
        pos2.setX(pos1.x() - (x1 - x2)*lengthFactor);
    } else {
        pos1.setX(x1 + ((x2 - x1)*posPercentage_)/100);
        pos2.setX(pos1.x() + (x2 - x1)*lengthFactor);
    }
    if (y1 > y2) {
        pos1.setY(y1 - ((y1 - y2)*posPercentage_)/100);
        pos2.setY(pos1.y() - (y1 - y2)*lengthFactor);
    } else {
        pos1.setY(y1 + ((y2 - y1)*posPercentage_)/100);
        pos2.setY(pos1.y() + (y2 - y1)*lengthFactor);
    }

    return QPair<QPointF, QPointF>(pos1, pos2);
}
