#ifndef G_LANE_H
#define G_LANE_H

#include <QVariant>
#include <QGraphicsObject>
#include <QPen>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include "lane.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"

class G_Lane : public QGraphicsObject
{
    Q_OBJECT

public:
    G_Lane(QGraphicsItem *parent, Lane *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    ~G_Lane();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Lane* getModel() {return model_;}
    unsigned long getSegmentId() {return model_->getSegmentId();}
    unsigned int getLaneIndex() {return model_->getIndex();}

private:
    void createLane();
    void updateAnnotation();

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    Lane *model_;
    QPen penForLane_;
    QPen penForSidewalk_;
    QPainterPath shape_;
    QColor colorForSelected_;

    QBrush brush_;
    QPointF center_;
    QGraphicsSimpleTextItem *annotation_;
};

Q_DECLARE_METATYPE(G_Lane*)
#endif // G_LANE_H
