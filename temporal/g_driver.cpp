#include "g_driver.h"
#define WIDTH_SIZE
#define HEIGHT_SIZE

G_Driver::G_Driver(QGraphicsItem *parent, Driver *model, PreferenceManager *preferenceManager)
    : G_Agent(parent, preferenceManager) {
    model_ = model;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setVisible(true);

    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(10);
    pen_.setBrush(Qt::green);
    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(Qt::blue);
    image_ = QImage(iSimGUI::ICON_DIRECTORY + preferenceManager_->getDriverIcon());

    rect_ = QRect(0, 0, model_->getLength(), model_->getWidth());
    rect_.moveCenter(model_->getPos());

    //rotate the object about its center (both the rect and the pixmap)
    QTransform t;
    t.translate(model_->getPos().x(), model_->getPos().y());
    t.rotate(model_->getAngle());
    t.translate(-model_->getPos().x(), -model_->getPos().y());
    rect_ = t.mapRect(rect_);
    image_ = image_.transformed(t, Qt::SmoothTransformation);
}

Agent* G_Driver::getModel() {
    return model_;
}

QRectF G_Driver::boundingRect() const {
    return rect_;
}

void G_Driver::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawImage(rect_.toRect(), image_);
}
