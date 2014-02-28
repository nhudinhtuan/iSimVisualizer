#include "g_node.h"
#define DEFAULT_RADIUS 500
#define DEFAULT_X_OFFSET 1000
#define DEFAULT_Y_OFFSET 500

G_Node::G_Node(QGraphicsItem *parent, Node *model, PreferenceManager *preferenceManager,  MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), model_(model) {

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    colorForSelected_ = Qt::red;
    colorForSelected_.setAlpha(128);
    brush_.setStyle(Qt::SolidPattern);
    shape_.addEllipse(model_->getPos().toPoint(), DEFAULT_RADIUS, DEFAULT_RADIUS);

    annotation_ = new QGraphicsSimpleTextItem(this);
    QPointF& pos = model_->getPos();
    annotation_->setPos(pos.x() + DEFAULT_X_OFFSET, pos.y() + DEFAULT_Y_OFFSET);
    annotation_->setScale(50);
}

G_Node::~G_Node() {
    if (annotation_) delete annotation_;
}

QRectF G_Node::boundingRect() const {
    return shape_.boundingRect();
}
