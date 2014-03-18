#ifndef G_INCIDENT_H
#define G_INCIDENT_H

#include <QGraphicsItem>
#include "incident.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"
#include "geospatial/roadsegment.h"
#include "temporal/temporalindex.h"

class G_Incident : public QGraphicsObject {

public:
    explicit G_Incident(QGraphicsItem *parent, Incident *model, RoadSegment* roadSegment, PreferenceManager *preferenceManager,
                        MapGraphicsView *mapView, TemporalIndex *temporalIndex);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    TemporalIndex *temporalIndex_;
    RoadSegment* roadSegment_;
    Incident *incident_;

    QBrush brush_;
    QBrush hazardBrush_;
    QPen pen_;
    QPen hazardPen_;
    QPoint center_;
    QRectF rect_;
    QVector<QPair<QPointF, QPointF> > lanePos_;
    QImage image_;
    QPixmap brushImage_;
    QRectF imgRect_;
};

Q_DECLARE_METATYPE(G_Incident*)
#endif // G_INCIDENT_H
