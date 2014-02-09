#ifndef G_LANE_H
#define G_LANE_H

#include <QGraphicsObject>
#include <QPen>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include "lane.h"
#include "preferencemanager.h"

class G_Lane : public QGraphicsObject
{
    Q_OBJECT

public:
    G_Lane(QGraphicsItem *parent, Lane *model, PreferenceManager *preferenceManager);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void createLane();

protected:
    PreferenceManager *preferenceManager_;
    Lane *model_;
    QPen penForLane_;
    QPen penForSidewalk_;
    QPainterPath shape_;
    QColor colorForSelected_;
};

#endif // G_LANE_H
