#include "crossing.h"

const int Crossing::DB_ID = 5;

Crossing::Crossing(unsigned long id) {
    id_ = id;
}

Crossing::Crossing(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList polyline = dbInfo.split("&&");
    for (int i = 0; i < polyline.size(); i++) {
        QStringList pointStr = polyline[i].split(":");
        QPointF point(pointStr[0].toDouble(), -(pointStr[1].toDouble()));
        polyline_.append(point);
    }
}

Crossing::~Crossing() {
}

QString Crossing::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList polyline;
    for (int i = 0; i < polyline_.size(); i++) {
        const QPointF& p = polyline_.at(i);
        polyline << QString::number(p.x(), 'f', 2) + ":" + QString::number(-p.y(), 'f', 2);
    }

    result += ", \'" + polyline.join("&&") + "\'";
    result += ")";
    return result;
}
