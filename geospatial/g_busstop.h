#ifndef G_BUSSTOP_H
#define G_BUSSTOP_H

#include <QGraphicsObject>
#include <QImage>
#include <QRect>
#include <QPainter>
#include <QDebug>
#include "preferencemanager.h"
#include "busstop.h"

class G_BusStop: public QGraphicsObject
{
    Q_OBJECT

public:
    explicit G_BusStop(QGraphicsItem *parent, BusStop *model, PreferenceManager *preferenceManager);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    PreferenceManager *preferenceManager_;
    BusStop *model_;

    QImage image_;
    QImage imageSelected_;
    QRect rect_;
};

#endif // G_BUSSTOP_H
