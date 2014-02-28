#ifndef CROSSING_H
#define CROSSING_H

#include <QVector>
#include <QPointF>

class Crossing
{
public:
    Crossing(unsigned long id);
    ~Crossing();
    unsigned long getId() { return id_; }
    void addPointToPolyline(QPointF p) {polyline_.append(p);}
    QVector<QPointF>& getPolyline() {return polyline_;}

private:
    unsigned long id_;
    QVector<QPointF> polyline_;
};

#endif // CROSSING_H
