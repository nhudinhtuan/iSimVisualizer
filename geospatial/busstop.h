#ifndef BUSSTOP_H
#define BUSSTOP_H

#include <QPointF>

class BusStop
{
public:
    BusStop(unsigned long id, unsigned long aimsunId, QPointF pos);
    unsigned long getId() {return id_;}
    unsigned long getAimsunId() { return aimsunId_;}
    QPointF& getPos() {return pos_;}

private:
    unsigned long id_;
    unsigned long aimsunId_;
    QPointF pos_;

};

#endif // BUSSTOP_H
