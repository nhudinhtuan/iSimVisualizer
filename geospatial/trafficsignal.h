#ifndef TRAFFICSIGNAL_H
#define TRAFFICSIGNAL_H

#include <QList>
#include <QVector>
#include <QPointF>
#include <QHash>
#include <QString>
#include <QStringList>

struct TrafficPhase {
    QString name;
    QVector<QPair<unsigned long, unsigned long> > segments;
    QVector<QPair<QPointF, QPointF> > lines;
    QList<unsigned long> crossings;
    TrafficPhase(QString& str, bool isCompleted = false) {
        if (isCompleted == false) {
            name = str;
        } else {
            QStringList attributes = str.split("$$");
            name = attributes[0];

            QStringList segmentStrs = attributes[1].split("&&");
            for (int i = 0; i < segmentStrs.size(); i++) {
                QStringList pairStr = segmentStrs[i].split("|");
                QPair<unsigned long, unsigned long> pair(pairStr[0].toULongLong(), pairStr[1].toULongLong());
                segments.append(pair);
            }

            QStringList crossingStrs = attributes[2].split("&&");
            for (int i = 0; i < crossingStrs.size(); i++) {
                crossings.append(crossingStrs[i].toULongLong());
            }
        }
    }

    QString serialize() {
        QStringList info;
        info << name;

        QStringList segmentStrs;
        for (int i = 0; i < segments.size(); i++) {
            segmentStrs << (QString::number(segments[i].first) + "|" + QString::number(segments[i].second));
        }
        info << segmentStrs.join("&&");

        QStringList crossingStrs;
        for (int i = 0; i < crossings.size(); i++) {
            crossingStrs << QString::number(crossings[i]);
        }
        info << crossingStrs.join("&&");

        return info.join("$$");
    }
};

class TrafficSignal
{
public:
    static const int DB_ID;
    TrafficSignal(unsigned long id, unsigned long nodeId);
    TrafficSignal(unsigned long id, QString& dbInfo);
    ~TrafficSignal();
    unsigned long getId() { return id_;}
    unsigned long getNodeId() { return nodeId_;}
    QList<TrafficPhase*> getPhases() { return phases_.values();}
    TrafficPhase* getPhase(QString name);

    void addPhase(QString name, TrafficPhase*);

    QString sqlInsertValue() const;

private:
    unsigned long id_;
    unsigned long nodeId_;
    QHash<QString, TrafficPhase*> phases_;
};

#endif // TRAFFICSIGNAL_H
