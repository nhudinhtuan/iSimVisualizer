#include "g_lane.h"
#define LANE_WIDTH 20
#define SIDEWALK_WIDTH 50

G_Lane::G_Lane(QGraphicsItem *parent, Lane *model, PreferenceManager *preferenceManager)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), model_(model) {

    //setFlag(QGraphicsItem::ItemIsSelectable, true);

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

    createLane();
}

void G_Lane::createLane() {
    QVector<QPointF>& polyline = model_->getPolyline();
    if (polyline.size() > 0) {
        shape_.moveTo(polyline[0]);
        for (int i = 1; i < polyline.size(); i++)
            shape_.lineTo(polyline[i]);
    }
}

QRectF G_Lane::boundingRect() const {
    return shape_.boundingRect();
}

void G_Lane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (isSelected()) {
        penForLane_.setColor(colorForSelected_);
        penForSidewalk_.setColor(colorForSelected_);
    }
    else {
        penForLane_.setColor(Qt::black);
        penForSidewalk_.setColor(Qt::black);
    }

    painter->setPen(model_->isSideWalk() ? penForSidewalk_ : penForLane_);
    painter->drawPath(shape_);
}
