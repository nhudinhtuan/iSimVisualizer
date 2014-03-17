#ifndef BUSSTOP_H
#define BUSSTOP_H

#include <QPointF>
#include <QString>
#include <QStringList>

class BusStop
{
public:
    static const int DB_ID;
    BusStop(unsigned long id, unsigned long aimsunId, QPointF pos);
    BusStop(unsigned long id, QString& dbInfo);
    unsigned long getId() {return id_;}
    unsigned long getAimsunId() { return aimsunId_;}
    QPointF& getPos() {return pos_;}

    QString sqlInsertValue() const;

private:
    unsigned long id_;
    unsigned long aimsunId_;
    QPointF pos_;

};

#endif // BUSSTOP_H
