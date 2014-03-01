#ifndef G_CROSSING_H
#define G_CROSSING_H

#include <QVariant>
#include <QGraphicsObject>
#include <QBrush>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include "crossing.h"
#include "preferencemanager.h"
#include "temporal/temporalindex.h"
#include "graphicsview/mapgraphicsview.h"

class G_Crossing: public QGraphicsObject
{
    Q_OBJECT

public:
    G_Crossing(QGraphicsItem *parent, Crossing *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView, TemporalIndex *temporalIndex);
    ~G_Crossing();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned long getId() { return model_->getId(); }
    Crossing* getModel() {return model_;}

private:
    void createCrossing();
    void updateAnnotation();
    void updateColor();

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    TemporalIndex *temporalIndex_;
    Crossing *model_;
    QPainterPath shape_;

    QBrush brush_;
};
Q_DECLARE_METATYPE(G_Crossing*)
#endif // G_CROSSING_H
