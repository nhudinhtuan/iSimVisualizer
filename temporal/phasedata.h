#ifndef PHASEDATA_H
#define PHASEDATA_H

#include <QString>
#include <QVector>
#include <QStringList>

struct CrossingPhaseData {
    unsigned int tick;
    unsigned long crossingId;
    unsigned int color;
    unsigned long nodeId;
    CrossingPhaseData() {
       tick = 0;
       crossingId = 0;
       color = 0;
       nodeId = 0;
    }

    CrossingPhaseData(unsigned int tick, unsigned long crossingId, unsigned int color, unsigned long nodeId)
        : tick(tick), crossingId(crossingId), color(color), nodeId(nodeId){
    }
    CrossingPhaseData(const CrossingPhaseData& other) {
        tick = other.tick;
        crossingId = other.crossingId;
        color = other.color;
        nodeId = other.nodeId;
    }
    QString sqlInsertValue() {
        QString result = "(";
        result += "1";
        result += ", " + QString::number(crossingId);
        result += ", " + QString::number(tick);
        result += ", \'\'";
        result += ", \'" + QString::number(color) + "\'";
        result += ", " + QString::number(nodeId);
        result += ")";
        return result;
    }

    CrossingPhaseData& operator=(const CrossingPhaseData& other ) {
        tick = other.tick;
        crossingId = other.crossingId;
        color = other.color;
        nodeId = other.nodeId;
        return *this;
    }
};

struct TrafficPhaseData {
    unsigned int tick;
    unsigned long id;
    QString name;
    QVector<unsigned int> colors;
    unsigned long nodeId;
    TrafficPhaseData() {
        tick = 0;
        id = 0;
        name = "";
        nodeId = 0;
    }

    TrafficPhaseData(unsigned long id, unsigned int tick, QString name, unsigned long nodeId) : tick(tick), id(id), name(name), nodeId(nodeId) {

    }
    TrafficPhaseData (const TrafficPhaseData& other) {
        tick = other.tick;
        id = other.id;
        name = other.name;
        colors = other.colors;
        nodeId = other.nodeId;
    }

    TrafficPhaseData& operator=(const TrafficPhaseData& other ) {
        tick = other.tick;
        id = other.id;
        name = other.name;
        colors = other.colors;
        nodeId = other.nodeId;
        return *this;
    }

    QString sqlInsertValue() {
        QString result = "(";
        result += "2";
        result += "," + QString::number(id);
        result += ", " + QString::number(tick);
        result += ", \'" + name + "\'";
        QStringList colorsStr;
        for (int i = 0; i < colors.size(); i++)
            colorsStr << QString::number(colors[i]);
        result += ", \'" + colorsStr.join(",") + "\'";
        result += ", " + QString::number(nodeId);
        result += ")";
        return result;
    }
};

#endif // PHASEDATA_H
