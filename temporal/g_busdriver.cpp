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

QList<QTableWidgetItem*> G_BusDriver::buildPropertyTable() {
    QList<QTableWidgetItem*> property;
    property.append(new QTableWidgetItem("Type "));
    property.append(new QTableWidgetItem("Pedestrian "));
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
    property.append(new QTableWidgetItem("Passenger "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getPassenger())));
    property.append(new QTableWidgetItem("Real Arrival Time "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getRealArrivalTime())));
    property.append(new QTableWidgetItem("DwellTime "));
    property.append(new QTableWidgetItem(tr("%1").arg(model_.getDwellTime())));
    property.append(new QTableWidgetItem("Bus Line Id "));
    property.append(new QTableWidgetItem(model_.getBusLineID()));

    return property;
}
