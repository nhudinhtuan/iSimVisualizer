#ifndef ROADSEGMENT_H
#define ROADSEGMENT_H

#include <QVector>
#include <QPointF>
#include <QDebug>
#include "lane.h"

class RoadSegment
{
public:
    static const int DB_ID;
    RoadSegment(unsigned long linkId, unsigned long id, unsigned long aimsunId, unsigned long fromNodeId,
                unsigned long toNodeId, int maxSpeed, int width, int nLane);
    RoadSegment(unsigned long id, QString& dbInfo);
    ~RoadSegment();
    unsigned long getId() {return id_;}
    unsigned long getAimsunId() {return aimsunId_;}
    unsigned long getFromNodeId() {return fromNodeId_;}
    unsigned long getToNodeId() {return toNodeId_;}
    unsigned long getlinkId() {return linkId_;}
    int getMaxSpeed() {return maxSpeed_;}
    int getWidth() {return width_;}
    int getNLane() {return nLane_;}
    QVector<QPointF>& getPolyline() {return polyline_;}
    QVector<Lane*>& getLanes() {return lanes_;}
    Lane* getLane(int index);
    Lane* getMiddleLane();

    void addPointToPolyline(QPointF p) {polyline_.append(p);}
    void addLane(Lane *lane) {lanes_.insert(lane->getIndex(), lane);}

    QString sqlInsertValue() const;

private:
    unsigned long linkId_;
    unsigned long id_;
    unsigned long aimsunId_;
    unsigned long fromNodeId_;
    unsigned long toNodeId_;
    int maxSpeed_;
    int width_;
    int nLane_;
    QVector<QPointF> polyline_;
    QVector<Lane*> lanes_;
};

#endif // ROADSEGMENT_H
