#include "g_busdriver.h"
#define WIDTH_SIZE
#define HEIGHT_SIZE

G_BusDriver::G_BusDriver(QGraphicsItem *parent, BusDriver *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : G_Agent(parent, preferenceManager, mapView) {

    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(10);
    pen_.setBrush(Qt::green);
    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(Qt::blue);

    updateModel(model);
}

void G_BusDriver::updateModel(Agent* model) {
    updateModel(dynamic_cast<BusDriver*>(model));
}

void G_BusDriver::updateModel(BusDriver *model) {
    if (!model) return;
    model_.update(model);

    prepareGeometryChange();
    image_ = QImage(iSimGUI::ICON_DIRECTORY + preferenceManager_->getBusIcon());

    rect_ = QRect(0, 0, model_.getLength(), model_.getWidth());
    rect_.moveCenter(model_.getPos());

    //rotate the object about its center (both the rect and the pixmap)
    QTransform t;
    t.translate(model_.getPos().x(), model_.getPos().y());
    t.rotate(model_.getAngle());
    t.translate(-model_.getPos().x(), -model_.getPos().y());
    rect_ = t.mapRect(rect_);
    image_ = image_.transformed(t, Qt::SmoothTransformation);
}

QRectF G_BusDriver::boundingRect() const {
    return rect_;
}

void G_BusDriver::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (this->isSelected()) {
        mapView_->centerOn(this);
    }
    painter->drawImage(rect_.toRect(), image_);
}
