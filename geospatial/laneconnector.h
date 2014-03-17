#ifndef LANECONNECTOR_H
#define LANECONNECTOR_H

#include <QPointF>
#include <QString>
#include <QStringList>

class LaneConnector
{
public:
    static const int DB_ID;
    LaneConnector(unsigned long id, unsigned long fromSegment, unsigned int fromLane, unsigned long toSegment, unsigned int toLane);
    LaneConnector(unsigned long id, QString& dbInfo);
    unsigned long getId() { return id_;}
    unsigned long getFromSegment() { return fromSegment_;}
    unsigned int getFromLane() { return fromLane_;}
    unsigned long getToSegment() { return toSegment_;}
    unsigned int getToLane() { return toLane_;}
    void setPoints(QPointF fromPoint, QPointF toPoint);

    QPointF& getFromPoint() { return fromPoint_;}
    QPointF& getToPoint() { return toPoint_;}

    QString sqlInsertValue() const;

private:
    unsigned long id_;
    unsigned long fromSegment_;
    unsigned int fromLane_; // from 0
    unsigned long toSegment_;
    unsigned int toLane_; // from 0

    QPointF fromPoint_;
    QPointF toPoint_;
};

#endif // LANECONNECTOR_H
