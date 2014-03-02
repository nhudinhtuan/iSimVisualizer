#ifndef G_SEGMENT_H
#define G_SEGMENT_H

#include <QVariant>
#include <QGraphicsObject>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include "roadsegment.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"

class G_Segment: public QGraphicsObject {
    Q_OBJECT

public:
    G_Segment(QGraphicsItem *parent, RoadSegment *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    ~G_Segment();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned long getSegmentId() { return model_->getId(); }
    RoadSegment* getModel() {return model_;}

private:
    void createSegment();
    void updateAnnotation();

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    RoadSegment *model_;
    QPainterPath shape_;
    QPainterPath baseSegmentShape_; // use when < threshold

    QBrush brush_;
    QPen pen_;
    QColor colorForSelected_;
    QPointF center_;
    QGraphicsSimpleTextItem *annotation_;
};

Q_DECLARE_METATYPE(G_Segment*)
#endif // G_SEGMENT_H
