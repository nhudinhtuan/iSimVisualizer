#include "g_crossing.h"

G_Crossing::G_Crossing(QGraphicsItem *parent, Crossing *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView, TemporalIndex *temporalIndex)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), temporalIndex_(temporalIndex), model_(model) {

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(QColor(200,10,10,200));
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

QPainterPath G_Crossing::shape () const {
    return shape_;
}

void G_Crossing::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isMicroscopicDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getMicroscopicThreshold()) {
        updateColor();
        painter->fillPath(shape_, brush_);
    }
}

void G_Crossing::updateColor() {
    if (temporalIndex_->isMicroDataExisted()) {
        int color = temporalIndex_->getCrossingPhaseColor(model_->getId());
        switch(color) {
            case 1: brush_.setColor(QColor(200,10,10,200));
                    break;
            case 2: brush_.setColor(QColor(200,150,10,200));
                    break;
            case 3: brush_.setColor(QColor(10,200,10,200));
                    break;
            default: break;
        }
    }
}
