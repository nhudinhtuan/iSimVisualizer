#include "g_segment.h"
#define USE_SEGMENT_POLYLINE 0

G_Segment::G_Segment(QGraphicsItem *parent, RoadSegment *model, PreferenceManager *preferenceManager)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), model_(model) {

    colorForSelected_ = Qt::magenta;
    colorForSelected_.setAlpha(128);
    brush_.setStyle(Qt::SolidPattern);
    createSegment();
}

void G_Segment::createSegment() {

#if USE_SEGMENT_POLYLINE == 0
    QVector<Lane*>& lanes = model_->getLanes();
    QVector<QPointF>& firstLanePoly = lanes.first()->getPolyline();
    QVector<QPointF>& lastLanePoly = lanes.last()->getPolyline();

    shape_.moveTo(firstLanePoly[0]);
    for (int i = 1; i < firstLanePoly.size(); i++)
        shape_.lineTo(firstLanePoly[i]);
    for (int i = lastLanePoly.size() - 1; i >= 0; i--)
        shape_.lineTo(lastLanePoly[i]);
#else
    QVector<QPointF>& segPolyline = model_->getPolyline();
    if (segPolyline.size() > 0) {
        shape_.moveTo(segPolyline[0]);
        for (int i = 1; i < segPolyline.size(); i++)
            shape_.lineTo(segPolyline[i]);
    }
#endif

}

QRectF G_Segment::boundingRect() const {
    return shape_.boundingRect();
}

void G_Segment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QColor color(Qt::blue);
    color.setAlpha(80);
    brush_.setColor(isSelected() ? colorForSelected_ : color);
    painter->fillPath(shape_, brush_);
}
