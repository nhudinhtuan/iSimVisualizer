#ifndef INCIDENT_H
#define INCIDENT_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QPointF>
#include <QtCore/qmath.h>

class Incident
{
public:
    static const int DB_ID;
    Incident(unsigned long id, unsigned long segment_aimsun_id, int posPercentage, int start_time, int duration,
             QList<int> lane, int visibility, int length);
    Incident(unsigned long id, QString& dbInfo);
    unsigned long getId(){ return id_;}
    unsigned long getSegmentAimSunId() { return segment_aimsun_id_; }
    int getStartTime() {return start_time_;}
    int getEndTime() { return start_time_+duration_;}
    int getDuration() {return duration_;}
    QList<int>& getLane() {return lane_;}
    int getVisibility() { return visibility_;}
    int getLength() { return length_;}
    QPair<QPointF, QPointF> getPosFromLane(qreal x1, qreal y1, qreal x2, qreal y2);

    QString sqlInsertValue() const;
private:
    unsigned long id_;
    unsigned long segment_aimsun_id_;
    int start_time_;
    int duration_;
    QList<int> lane_;
    int visibility_;
    int length_;
    int posPercentage_;
};

#endif // INCIDENT_H
