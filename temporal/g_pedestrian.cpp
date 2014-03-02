#include "g_pedestrian.h"
#define ICON_WIDTH 210
#define ICON_HEIGHT 200

G_Pedestrian::G_Pedestrian(QGraphicsItem *parent, Pedestrian *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
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

void G_Pedestrian::updateModel(Agent* model) {
    updateModel(dynamic_cast<Pedestrian*>(model));
}

void G_Pedestrian::updateModel(Pedestrian *model) {
    if (!model) return;
    model_.update(model);

    prepareGeometryChange();
    image_ = QImage(iSimGUI::ICON_DIRECTORY + preferenceManager_->getPedestrianIcon());
    rect_ = QRect(0, 0, ICON_WIDTH, ICON_HEIGHT);
    rect_.moveCenter(model_.getPos());
}

QRectF G_Pedestrian::boundingRect() const {
   return rect_;
}

void G_Pedestrian::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawImage(rect_.toRect(), image_);
}
