#include "g_segment.h"
#define USE_SEGMENT_POLYLINE 0

G_Segment::G_Segment(QGraphicsItem *parent, RoadSegment *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), model_(model) {

    //setFlag(QGraphicsItem::ItemIsSelectable, true);

    colorForSelected_ = Qt::magenta;
    colorForSelected_.setAlpha(128);
    brush_.setStyle(Qt::SolidPattern);
    pen_.setCapStyle(Qt::SquareCap);
    pen_.setJoinStyle(Qt::MiterJoin);
    pen_.setStyle(Qt::SolidLine);
    createSegment();

    annotation_ = new QGraphicsSimpleTextItem(this);
    annotation_->setScale(40);
    annotation_->setPos(center_);
}

G_Segment::~G_Segment() {
    if (annotation_) delete annotation_;
}

void G_Segment::createSegment() {

    qreal maxX = 0, minX = 0, maxY = 0, minY = 0;
#if USE_SEGMENT_POLYLINE == 0
    QVector<Lane*>& lanes = model_->getLanes();
    QVector<QPointF>& firstLanePoly = lanes.first()->getPolyline();
    QVector<QPointF>& lastLanePoly = lanes.last()->getPolyline();

    minX = maxX = firstLanePoly[0].x();
    minY = maxY = firstLanePoly[0].y();
    shape_.moveTo(firstLanePoly[0]);
    for (int i = 1; i < firstLanePoly.size(); i++) {
        shape_.lineTo(firstLanePoly[i]);
        minX = minX < firstLanePoly[i].x() ? minX : firstLanePoly[i].x();
        maxX = maxX > firstLanePoly[i].x() ? maxX : firstLanePoly[i].x();
        minY = minY < firstLanePoly[i].y() ? minY : firstLanePoly[i].y();
        maxY = maxY > firstLanePoly[i].y() ? maxY : firstLanePoly[i].y();
    }
    for (int i = lastLanePoly.size() - 1; i >= 0; i--) {
        shape_.lineTo(lastLanePoly[i]);
        minX = minX < lastLanePoly[i].x() ? minX : lastLanePoly[i].x();
        maxX = maxX > lastLanePoly[i].x() ? maxX : lastLanePoly[i].x();
        minY = minY < lastLanePoly[i].y() ? minY : lastLanePoly[i].y();
        maxY = maxY > lastLanePoly[i].y() ? maxY : lastLanePoly[i].y();
    }

    // create a base segment line
    qreal p1x = (firstLanePoly.first().x() + lastLanePoly.first().x())/2;
    qreal p1y = (firstLanePoly.first().y() + lastLanePoly.first().y())/2;
    qreal p2x = (firstLanePoly.last().x() + lastLanePoly.last().x())/2;
    qreal p2y = (firstLanePoly.last().y() + lastLanePoly.last().y())/2;
    baseSegmentShape_.moveTo(p1x, p1y);
    baseSegmentShape_.lineTo(p2x, p2y);
#else
    QVector<QPointF>& segPolyline = model_->getPolyline();
    if (segPolyline.size() > 0) {
        minX = maxX = segPolyline[0].x();
        minY = maxY = segPolyline[0].y();
        shape_.moveTo(segPolyline[0]);
        for (int i = 1; i < segPolyline.size(); i++) {
            shape_.lineTo(segPolyline[i]);
            minX = minX < segPolyline[i].x() ? minX : segPolyline[i].x();
            maxX = maxX > segPolyline[i].x() ? maxX : segPolyline[i].x();
            minY = minY < segPolyline[i].y() ? minY : segPolyline[i].y();
            maxY = maxY > segPolyline[i].y() ? maxY : segPolyline[i].y();
        }
    }
    baseSegmentShape_ = shape_;
#endif
    center_.setX((minX+maxX)/2);
    center_.setY((minY+maxY)/2);
}

QRectF G_Segment::boundingRect() const {
    return shape_.boundingRect();
}

void G_Segment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QColor color(preferenceManager_->getSegmentColor());
    if (preferenceManager_->isSegmentDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getSegmentThreshold()) {
        color.setAlpha(90);
        brush_.setColor(isSelected() ? colorForSelected_ : color);
        painter->fillPath(shape_, brush_);

        if (preferenceManager_->getSegmentExtraInfo()) {
            annotation_->setBrush(brush_);
            updateAnnotation();
            annotation_->setVisible(true);
        } else {
            annotation_->setVisible(false);
        }
    } else {
        pen_.setColor(isSelected() ? colorForSelected_ : color);
        pen_.setWidth(model_->getWidth());
        painter->setPen(pen_);
        painter->drawPath(baseSegmentShape_);
        annotation_->setVisible(false);
    }
}

void G_Segment::updateAnnotation() {
    switch (preferenceManager_->getSegmentExtraInfo()) {
        case 0: { // no annotation
            annotation_->setText("");
            break;
        }
        case 1: { // segment ID
            annotation_->setText(QString::number(model_->getId()));
            break;
        }
        case 2: { // aimsun ID
            annotation_->setText(QString::number(model_->getAimsunId()));
            break;
        }
    }
}
