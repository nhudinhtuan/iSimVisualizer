#ifndef G_SEGMENT_H
#define G_SEGMENT_H

#include <QVariant>
#include <QGraphicsObject>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include "roadsegment.h"
#include "preferencemanager.h"
#include "temporal/temporalindex.h"
#include "graphicsview/mapgraphicsview.h"

class G_Segment: public QGraphicsObject {
    Q_OBJECT

public:
    explicit G_Segment(QGraphicsItem *parent, RoadSegment *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView, TemporalIndex *temporalIndex);
    ~G_Segment();
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape () const;
    unsigned long getSegmentId() { return model_->getId(); }
    RoadSegment* getModel() {return model_;}

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void announceDoubleClick(unsigned long segmentId);

private:
    void createSegment();
    void updateAnnotation();
    void updateColor();

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    TemporalIndex *temporalIndex_;
    RoadSegment *model_;
    QPainterPath shape_;
    QPainterPath baseSegmentShape_; // use when < threshold
    bool useShape_;

    QBrush brush_;
    QPen pen_;
    QColor color_;
    QColor colorForSelected_;
    QPointF center_;
    QGraphicsSimpleTextItem *annotation_;
};

Q_DECLARE_METATYPE(G_Segment*)
#endif // G_SEGMENT_H
