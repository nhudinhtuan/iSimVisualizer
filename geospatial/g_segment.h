#ifndef G_SEGMENT_H
#define G_SEGMENT_H

#include <QGraphicsObject>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include "roadsegment.h"
#include "preferencemanager.h"

class G_Segment: public QGraphicsObject {
    Q_OBJECT

public:
    G_Segment(QGraphicsItem *parent, RoadSegment *model, PreferenceManager *preferenceManager);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void createSegment();

protected:
    PreferenceManager *preferenceManager_;
    RoadSegment *model_;
    QPainterPath shape_;

    QBrush brush_;
    QColor colorForSelected_;
};

#endif // G_SEGMENT_H
