#ifndef G_LANECONNECTOR_H
#define G_LANECONNECTOR_H

#include <QVariant>
#include <QGraphicsObject>
#include <QPen>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include <QPolygonF>
#include "laneconnector.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"

class G_LaneConnector : public QGraphicsObject
{
    Q_OBJECT

public:
    G_LaneConnector(QGraphicsItem *parent, LaneConnector *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    ~G_LaneConnector();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    LaneConnector* getModel() {return model_;}

private:
    void createLaneConnector();

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    LaneConnector *model_;
    QPen pen_;
    QBrush brush_;
    QPainterPath curve_;
    QPainterPath arrow_;
};

Q_DECLARE_METATYPE(G_LaneConnector*)

#endif // G_LANECONNECTOR_H
