#include "g_lane.h"
#define LANE_WIDTH 20
#define SIDEWALK_WIDTH 50

G_Lane::G_Lane(QGraphicsItem *parent, Lane *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), model_(model) {

    setFlag(QGraphicsItem::ItemIsSelectable, true);

    penForLane_.setCapStyle(Qt::RoundCap);
    penForLane_.setJoinStyle(Qt::MiterJoin);
    penForLane_.setStyle(Qt::DotLine);
    penForLane_.setWidth(LANE_WIDTH);

    penForSidewalk_.setCapStyle(Qt::RoundCap);
    penForSidewalk_.setJoinStyle(Qt::MiterJoin);
    penForSidewalk_.setStyle(Qt::SolidLine);
    penForSidewalk_.setWidth(SIDEWALK_WIDTH);

    colorForSelected_ = Qt::magenta;
    colorForSelected_.setAlpha(128);

    brush_.setStyle(Qt::SolidPattern);

    createLane();
    annotation_ = new QGraphicsSimpleTextItem(this);
    annotation_->setScale(20);
    annotation_->setPos(center_);
}

G_Lane::~G_Lane() {
    if (annotation_) delete annotation_;
}

void G_Lane::createLane() {
    qreal minX = 0, maxX = 0, minY = 0, maxY = 0;
    QVector<QPointF>& polyline = model_->getPolyline();
    if (polyline.size() > 0) {
        minX = maxX = polyline[0].x();
        minY = maxY = polyline[0].y();
        shape_.moveTo(polyline[0]);
        for (int i = 1; i < polyline.size(); i++) {
            shape_.lineTo(polyline[i]);
            minX = minX < polyline[i].x() ? minX : polyline[i].x();
            maxX = maxX > polyline[i].x() ? maxX : polyline[i].x();
            minY = minY < polyline[i].y() ? minY : polyline[i].y();
            maxY = maxY > polyline[i].y() ? maxY : polyline[i].y();
        }
    }
    center_.setX((minX+maxX)/2);
    center_.setY((minY+maxY)/2);
}

QRectF G_Lane::boundingRect() const {
    return shape_.boundingRect();
}

QPainterPath G_Lane::shape () const {
    return shape_;
}

void G_Lane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isLaneDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getLaneThreshold()) {
        if (isSelected()) {
            brush_.setColor(colorForSelected_);
            penForLane_.setColor(colorForSelected_);
            penForSidewalk_.setColor(colorForSelected_);
        }
        else {
            QColor& color = preferenceManager_->getLaneColor();
            brush_.setColor(color);
            penForLane_.setColor(color);
            penForSidewalk_.setColor(color);
        }

        painter->setPen(model_->isSideWalk() ? penForSidewalk_ : penForLane_);
        painter->drawPath(shape_);

        if (preferenceManager_->getLaneExtraInfo()) {
            annotation_->setBrush(brush_);
            updateAnnotation();
            annotation_->setVisible(true);
        } else {
            annotation_->setVisible(false);
        }
    } else {
        annotation_->setVisible(false);
    }
}

void G_Lane::updateAnnotation() {
    switch (preferenceManager_->getLaneExtraInfo()) {
        case 0: { // no annotation
            annotation_->setText("");
            break;
        }
        case 1: { // lane number
            annotation_->setText(QString::number(model_->getIndex()));
            break;
        }
    }
}
