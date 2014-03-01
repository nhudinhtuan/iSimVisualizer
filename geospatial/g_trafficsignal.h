#ifndef G_TRAFFICSIGNAL_H
#define G_TRAFFICSIGNAL_H

#include <QVariant>
#include <QGraphicsObject>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QHash>
#include <QtCore/qmath.h>
#include "trafficsignal.h"
#include "preferencemanager.h"
#include "temporal/temporalindex.h"
#include "graphicsview/mapgraphicsview.h"

class G_TrafficSignal: public QGraphicsObject {
    Q_OBJECT

public:
    G_TrafficSignal(QGraphicsItem *parent, TrafficSignal *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView, TemporalIndex *temporalIndex);
    ~G_TrafficSignal();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned long getId() { return model_->getId(); }
    TrafficSignal* getModel() {return model_;}

private:
    void createShape();
    void setColor(unsigned int);

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    TemporalIndex *temporalIndex_;
    TrafficSignal* model_;

    QHash<QString, QVector<QPainterPath> > lines_;
    QHash<QString, QVector<QPainterPath> > arrows_;
    QPainterPath shape_;

    QBrush brush_;
    QPen pen_;
};
Q_DECLARE_METATYPE(G_TrafficSignal*)
#endif // G_TRAFFICSIGNAL_H
