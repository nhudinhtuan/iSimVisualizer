#ifndef BUSSTOP_H
#define BUSSTOP_H

#include <QPointF>

class BusStop
{
public:
    BusStop(unsigned long id, QPointF pos);
    unsigned long getId() {return id_;}
    QPointF& getPos() {return pos_;}

private:
    unsigned long id_;
    QPointF pos_;

};

#endif // BUSSTOP_H
