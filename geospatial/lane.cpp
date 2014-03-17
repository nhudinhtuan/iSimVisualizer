#include "lane.h"

Lane::Lane(unsigned long segmentId, unsigned int index, bool isSideWalk)
{
    segmentId_ = segmentId;
    index_ = index;
    isSideWalk_ = isSideWalk;
}

Lane::Lane(QString& str) {
    QStringList info = str.split("$$");
    index_ = info[0].toInt();
    if (info[1] == "TRUE") isSideWalk_ = true;
    else isSideWalk_ = false;

    QStringList polyline = info[2].split("&&");
    for (int i = 0; i < polyline.size(); i++) {
        QStringList pointStr = polyline[i].split(":");
        QPointF point(pointStr[0].toDouble(), -(pointStr[1].toDouble()));
        polyline_.append(point);
    }
}

QPointF Lane::getFirstPoint() {
    if (polyline_.size() == 0) return QPointF(0, 0);
    return polyline_[0];
}

QPointF Lane::getLastPoint() {
    if (polyline_.size() == 0) return QPointF(0, 0);
    return polyline_[polyline_.size()-1];
}

QString Lane::serialize() const {
    QStringList info;
    info << QString::number(index_);
    info << (isSideWalk_? "TRUE" : "FALSE");

    QStringList polyline;
    for (int i = 0; i < polyline_.size(); i++) {
        const QPointF& p = polyline_.at(i);
        polyline << QString::number(p.x(), 'f', 2) + ":" + QString::number(-p.y(), 'f', 2);
    }
    info << polyline.join("&&");

    return info.join("$$");
}
