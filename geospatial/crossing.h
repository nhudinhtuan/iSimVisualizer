#ifndef CROSSING_H
#define CROSSING_H

#include <QVector>
#include <QPointF>
#include <QString>
#include <QStringList>

class Crossing
{
public:
    static const int DB_ID;
    Crossing(unsigned long id);
    Crossing(unsigned long id, QString& dbInfo);
    ~Crossing();
    unsigned long getId() { return id_; }
    void addPointToPolyline(QPointF p) {polyline_.append(p);}
    QVector<QPointF>& getPolyline() {return polyline_;}

    QString sqlInsertValue() const;

private:
    unsigned long id_;
    QVector<QPointF> polyline_;
};

#endif // CROSSING_H
