#include "g_crossing.h"

G_Crossing::G_Crossing(QGraphicsItem *parent, Crossing *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), model_(model) {

    //setFlag(QGraphicsItem::ItemIsSelectable, true);
    brush_.setStyle(Qt::SolidPattern);
    createCrossing();
}

G_Crossing::~G_Crossing() {

}

void G_Crossing::createCrossing() {
    QVector<QPointF>& polyline = model_->getPolyline();
    if (polyline.size() > 0) {
        shape_.moveTo(polyline[0]);
        for (int i = 1; i < polyline.size(); i++) {
            shape_.lineTo(polyline[i]);
        }
    }
}

QRectF G_Crossing::boundingRect() const {
    return shape_.boundingRect();
}

void G_Crossing::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isCrossingDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getCrossingThreshold()) {
        brush_.setColor(Qt::red);
        painter->fillPath(shape_, brush_);
    }
}
