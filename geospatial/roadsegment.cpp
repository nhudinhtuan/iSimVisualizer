#include "roadsegment.h"

const int RoadSegment::DB_ID = 2;

RoadSegment::RoadSegment(unsigned long linkId, unsigned long id, unsigned long aimsunId, unsigned long fromNodeId,
                         unsigned long toNodeId, int maxSpeed, int width, int nLane) {
    linkId_ = linkId;
    id_ = id;
    aimsunId_ = aimsunId;
    fromNodeId_ = fromNodeId;
    toNodeId_ = toNodeId;
    maxSpeed_ = maxSpeed;
    width_ = width;
    nLane_ = nLane;
}

RoadSegment::RoadSegment(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    linkId_ = attributes[0].toULongLong();
    aimsunId_ = attributes[1].toULongLong();
    fromNodeId_ = attributes[2].toULongLong();
    toNodeId_ = attributes[3].toULongLong();
    maxSpeed_ = attributes[4].toInt();
    width_ = attributes[5].toInt();
    nLane_ = attributes[6].toInt();

    QStringList polyline = attributes[7].split("&&");
    for (int i = 0; i < polyline.size(); i++) {
        QStringList pointStr = polyline[i].split(":");
        QPointF point(pointStr[0].toDouble(), -(pointStr[1].toDouble()));
        polyline_.append(point);
    }

    QStringList lanes = attributes[8].split(",");
    for (int i = 0; i < lanes.size(); i++) {
        Lane *lane = new Lane(lanes[i]);
        lanes_.append(lane);
    }
}

RoadSegment::~RoadSegment() {
    for (int i = 0; i < lanes_.size(); ++i) {
        delete lanes_[i];
    }
}

Lane* RoadSegment::getLane(int index) {
    if (index < 0 || lanes_.size() == 0 || index + 1 > lanes_.size())
        return 0;
    return lanes_[index];
}

Lane* RoadSegment::getMiddleLane() {
    if (lanes_.size() == 0) return 0;
    return lanes_[lanes_.size()/2];
}

QString RoadSegment::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList info;
    info << QString::number(linkId_);
    info << QString::number(aimsunId_);
    info << QString::number(fromNodeId_);
    info << QString::number(toNodeId_);
    info << QString::number(maxSpeed_);
    info << QString::number(width_);
    info << QString::number(nLane_);

    QStringList polyline;
    for (int i = 0; i < polyline_.size(); i++) {
        const QPointF& p = polyline_.at(i);
        polyline << QString::number(p.x(), 'f', 2) + ":" + QString::number(-p.y(), 'f', 2);
    }
    info << polyline.join("&&");

    QStringList lanes;
    for (int i = 0; i < lanes_.size(); i++) {
        lanes << lanes_.at(i)->serialize();
    }
    info << lanes.join(",");

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}
