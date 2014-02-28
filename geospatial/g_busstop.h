#ifndef G_BUSSTOP_H
#define G_BUSSTOP_H

#include <QVariant>
#include <QGraphicsObject>
#include <QImage>
#include <QRect>
#include <QPainter>
#include <QDebug>
#include "graphicsview/mapgraphicsview.h"
#include "preferencemanager.h"
#include "busstop.h"

class G_BusStop: public QGraphicsObject
{
    Q_OBJECT

public:
    explicit G_BusStop(QGraphicsItem *parent, BusStop *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned long getModelId() { return model_->getId(); }
    BusStop* getModel() { return model_;}

protected:
    MapGraphicsView *mapView_;
    PreferenceManager *preferenceManager_;
    BusStop *model_;

    QImage image_;
    QImage imageSelected_;
    QRect rect_;
};
Q_DECLARE_METATYPE(G_BusStop*)
#endif // G_BUSSTOP_H
