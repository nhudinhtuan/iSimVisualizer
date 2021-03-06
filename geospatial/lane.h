#ifndef LANE_H
#define LANE_H

#include <QVector>
#include <QPointF>
#include <QString>
#include <QStringList>

class Lane
{
public:
    Lane(unsigned long segmentId, unsigned int index, bool isSideWalk);
    Lane(QString& str);
    unsigned long getSegmentId() {return segmentId_;}
    unsigned int getIndex() {return index_;}
    QVector<QPointF>& getPolyline() {return polyline_;}
    void addPointToPolyline(QPointF p) {polyline_.append(p);}
    bool isSideWalk() {return isSideWalk_;}

    QPointF getFirstPoint();
    QPointF getLastPoint();

    QString serialize() const;

private:
    unsigned long segmentId_;
    unsigned int index_;
    QVector<QPointF> polyline_;
    bool isSideWalk_;
};

#endif // LANE_H
