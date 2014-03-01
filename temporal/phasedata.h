#ifndef PHASEDATA_H
#define PHASEDATA_H

#include <QString>
#include <QVector>

struct CrossingPhaseData {
    unsigned int tick;
    unsigned long crossingId;
    unsigned int color;
    CrossingPhaseData(unsigned int tick, unsigned long crossingId, unsigned int color)
        : tick(tick), crossingId(crossingId), color(color){
    }
};

struct TrafficPhaseData {
    unsigned int tick;
    unsigned long id;
    QString name;
    QVector<unsigned int> colors;
    TrafficPhaseData(unsigned long id, unsigned int tick, QString name) : tick(tick), id(id), name(name) {

    }
};

#endif // PHASEDATA_H
