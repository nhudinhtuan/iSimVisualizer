#include "g_busstop.h"
#define RECT_SIZE 500

G_BusStop::G_BusStop(QGraphicsItem *parent, BusStop *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), mapView_(mapView), preferenceManager_(preferenceManager), model_(model)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    image_ = QImage(":/ui_icons/bus_blue.png");
    imageSelected_ = QImage (":/ui_icons/bus_selected.png");

    rect_ = QRect(0, 0, RECT_SIZE, RECT_SIZE);
    rect_.moveCenter(model_->getPos().toPoint());
}

QRectF G_BusStop::boundingRect() const {
    return rect_;
}

void G_BusStop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isBusStopDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getBusstopThreshold()) {
        if (isSelected()) {
            painter->drawImage(rect_, imageSelected_);
        } else {
            painter->drawImage(rect_, image_);
        }
    }
}
