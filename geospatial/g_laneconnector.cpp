#include "g_laneconnector.h"
#define ARROW_SIZE 100.0
#define PI 3.14

G_LaneConnector::G_LaneConnector(QGraphicsItem *parent, LaneConnector *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), model_(model) {
    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::MiterJoin);
    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(20);
    brush_.setStyle(Qt::SolidPattern);
    createLaneConnector();
}

G_LaneConnector::~G_LaneConnector() {

}

QRectF G_LaneConnector::boundingRect() const {
    return curve_.boundingRect();
}


void G_LaneConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

   if (preferenceManager_->isLaneConnectorDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getLaneConnectorThreshold()) {
       QColor& color = preferenceManager_->getLaneConnectorColor();
       pen_.setColor(color);
       brush_.setColor(color);
       painter->setPen(pen_);
       painter->drawPath(curve_);
       painter->fillPath(arrow_, brush_);
   }

}

void G_LaneConnector::createLaneConnector() {
    curve_.moveTo(model_->getFromPoint());
    // in the future , shall use centreCurvePath.quadTo()
    curve_.lineTo(model_->getToPoint());

    // draw arrow head
    double angle = curve_.angleAtPercent(1)/57.3;
    QPointF arrowP1 = model_->getToPoint() - QPointF(sin(angle + PI / 3) * ARROW_SIZE,
                                    cos(angle + PI / 3) * ARROW_SIZE);
    QPointF arrowP2 = model_->getToPoint() - QPointF(sin(angle + PI - PI / 3) * ARROW_SIZE,
                                    cos(angle + PI - PI / 3) * ARROW_SIZE);
    arrow_.moveTo(model_->getToPoint());
    arrow_.lineTo(arrowP1);
    arrow_.lineTo(arrowP2);
}
