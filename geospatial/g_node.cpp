#include "g_node.h"
#define DEFAULT_RADIUS 1000

G_Node::G_Node(QGraphicsItem *parent, Node *model, PreferenceManager *preferenceManager)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), model_(model) {

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    colorForSelected_ = Qt::magenta;
    brush_.setStyle(Qt::SolidPattern);
    shape_.addEllipse(QPoint(model_->getXPos(), -model_->getYPos()), DEFAULT_RADIUS, DEFAULT_RADIUS);
}

QRectF G_Node::boundingRect() const {
    return shape_.boundingRect();
}

void G_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    setVisible(true);
    if (isSelected()) {
        brush_.setColor(colorForSelected_);
    } else {
        if (model_->getType() == iSimGUI::UNI_NODE)
            brush_.setColor(Qt::blue);
        else
            brush_.setColor(Qt::red);
    }

    painter->fillPath(shape_, brush_);
}
