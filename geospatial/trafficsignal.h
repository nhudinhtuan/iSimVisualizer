#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H

#include <QList>
#include <QVector>
#include <QPointF>
#include <QHash>
#include <QString>

struct TrafficPhase {
    QString name;
    QVector<QPair<unsigned long, unsigned long> > segments;
    QVector<QPair<QPointF, QPointF> > lines;
    QList<unsigned long> crossings;
    TrafficPhase(QString name) : name(name) {
    }
};

class TrafficSignal
{
public:
    TrafficSignal(unsigned long id, unsigned long nodeId);
    ~TrafficSignal();
    unsigned long getId() { return id_;}
    unsigned long getNodeId() { return nodeId_;}
    QList<TrafficPhase*> getPhases() { return phases_.values();}
    TrafficPhase* getPhase(QString name);

    void addPhase(QString name, TrafficPhase*);
private:
    unsigned long id_;
    unsigned long nodeId_;
    QHash<QString, TrafficPhase*> phases_;
};

#endif // TRAFFICSIGNAL_H
