#include "g_driver.h"
#define WIDTH_SIZE
#define HEIGHT_SIZE

G_Driver::G_Driver(QGraphicsItem *parent, Driver *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : G_Agent(parent, preferenceManager, mapView) {

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setVisible(true);

    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(10);
    pen_.setBrush(Qt::green);
    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(Qt::blue);

    updateModel(model);
}

void G_Driver::updateModel(Agent* model) {
    updateModel(dynamic_cast<Driver*>(model));
}

void G_Driver::updateModel(Driver *model) {
    if (!model) return;
    model_.update(model);

    prepareGeometryChange();
    rect_ = QRect(0, 0, model_.getLength(), model_.getWidth());
    rect_.moveCenter(model_.getPos());

    image_ = QImage(iSimGUI::ICON_DIRECTORY + preferenceManager_->getDriverIcon());
    //rotate the object about its center (both the rect and the pixmap)
    QTransform t;
    t.translate(model_.getPos().x(), model_.getPos().y());
    t.rotate(model_.getAngle());
    t.translate(-model_.getPos().x(), -model_.getPos().y());
    rect_ = t.mapRect(rect_);
    image_ = image_.transformed(t, Qt::SmoothTransformation);
}

QRectF G_Driver::boundingRect() const {
    return rect_;
}

void G_Driver::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (this->isSelected()) {
        mapView_->centerOn(this);
    }
    painter->drawImage(rect_.toRect(), image_);
}

QList<QTableWidgetItem*> G_Driver::buildPropertyTable() {
    QList<QTableWidgetItem*> property;
    property.append(new QTableWidgetItem("Type "));
    property.append(new QTableWidgetItem("Driver "));
    property.append(new QTableWidgetItem("Id "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getID())));
    property.append(new QTableWidgetItem("Tick "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getTick())));
    QPointF& pos = model_.getPos();
    property.append(new QTableWidgetItem("XPos "));
    property.append(new QTableWidgetItem(tr("%1").arg(pos.x())));
    property.append(new QTableWidgetItem("YPos "));
    property.append(new QTableWidgetItem(tr("%1").arg(-pos.y())));
    property.append(new QTableWidgetItem("Angle "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getAngle())));
    property.append(new QTableWidgetItem("Length "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getLength())));
    property.append(new QTableWidgetItem("Width "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getWidth())));
    property.append(new QTableWidgetItem("Current Segment "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getCurrentSegment())));
    property.append(new QTableWidgetItem("Fwd-speed "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getFwdSpeed())));
    property.append(new QTableWidgetItem("Fwd-accel "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getFwdAccel())));
    property.append(new QTableWidgetItem("Madatory "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getMandatory())));
    property.append(new QTableWidgetItem("Info "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getInfo())));
    return property;
}
