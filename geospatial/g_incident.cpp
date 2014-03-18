#include "g_incident.h"
#define CONG_ROAD_ICON ":/ui_icons/cong_road.png"
#define HAZARD_WARNING_ICON ":/ui_icons/hazard_warning.jpg"

G_Incident::G_Incident(QGraphicsItem *parent, Incident *model, RoadSegment* roadSegment, PreferenceManager *preferenceManager,
                       MapGraphicsView *mapView, TemporalIndex *temporalIndex)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView),
      temporalIndex_(temporalIndex), roadSegment_(roadSegment), incident_(model){

    image_ = QImage (CONG_ROAD_ICON);
    brushImage_ = QPixmap(HAZARD_WARNING_ICON);

    pen_.setColor(Qt::yellow);
    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(50);
    brush_.setStyle(Qt::NoBrush);

    hazardPen_.setStyle(Qt::NoPen);
    hazardBrush_ = QBrush(Qt::yellow, brushImage_);
    hazardBrush_.setStyle(Qt::TexturePattern);

    QVector<Lane*>& lanes = roadSegment_->getLanes();
    QList<int>& incidentLanes = incident_->getLane();
    for (int i = 0; i < incidentLanes.size(); i++) {
        Lane* lane1 = lanes[incidentLanes.at(i)];
        Lane* lane2 = lanes[incidentLanes.at(i)+1];
        QVector<QPointF>& lanePoints1 = lane1->getPolyline();
        QVector<QPointF>& lanePoints2 = lane2->getPolyline();

        lanePos_.push_back(incident_->getPosFromLane((lanePoints1[0].x()*2 + lanePoints2[0].x())/3,
                                                    (lanePoints1[0].y()*2 + lanePoints2[0].y())/3,
                                                    (lanePoints1[1].x()*2 + lanePoints2[1].x())/3,
                                                    (lanePoints1[1].y()*2 + lanePoints2[1].y())/3));
        lanePos_.push_back(incident_->getPosFromLane((lanePoints1[0].x() + lanePoints2[0].x()*2)/3,
                                                    (lanePoints1[0].y() + lanePoints2[0].y()*2)/3,
                                                    (lanePoints1[1].x() + lanePoints2[1].x()*2)/3,
                                                    (lanePoints1[1].y() + lanePoints2[1].y()*2)/3));
    }


    // find the position for image
    Lane* secondLastLane = lanes[lanes.size() - 2];
    Lane* lastLane = lanes[lanes.size() - 1];
    QVector<QPointF>& secondLastLanePoints = secondLastLane->getPolyline();
    QVector<QPointF>& lastLanePoints = lastLane->getPolyline();
    QPair<QPointF, QPointF> middleLine = incident_->getPosFromLane(lastLanePoints[0].x(), lastLanePoints[0].y(),
                                                                  lastLanePoints[1].x(), lastLanePoints[1].y());
    QPointF middlePoint((middleLine.first.x() + middleLine.second.x())/2,
                        (middleLine.first.y() + middleLine.second.y())/2);
    // ship middlePoint far from lastlane to become the place for icon image
    middlePoint.setX(middlePoint.x() + (lastLanePoints[0].x() - secondLastLanePoints[0].x())*3);
    middlePoint.setY(middlePoint.y() + (lastLanePoints[0].y() - secondLastLanePoints[0].y())*3);
    imgRect_ = QRect(middlePoint.x(), middlePoint.y(), 400, 400);
    imgRect_.moveCenter(QPoint(middlePoint.x(), middlePoint.y()));
    qDebug() << "middle point: " << middlePoint.x() << ", " << middlePoint.y();

    center_.setX(lanePos_[0].first.x());
    center_.setY(lanePos_[0].first.y());

    int visibility = incident_->getVisibility();
    if (visibility < 50)
        visibility = 50;
    QRectF temp(center_.x() - visibility, center_.y() - visibility, visibility*2, visibility*2);
    rect_ = temp;
}

QRectF G_Incident::boundingRect() const {
    return rect_;
}

void G_Incident::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    int currentTick = temporalIndex_->getCurrentTick();
    if (incident_->getStartTime() <= currentTick && incident_->getEndTime() >= currentTick) {

        // draw visibility
        painter->setPen(pen_);
        painter->setBrush(brush_);
        painter->drawEllipse(center_, incident_->getVisibility(), incident_->getVisibility());

        painter->setPen(hazardPen_);
        painter->setBrush(hazardBrush_);

        for (int i = 0; i < lanePos_.size(); i+=2) {
            QPointF points[4] = {
                 lanePos_[i].first,
                 lanePos_[i].second,
                 lanePos_[i+1].second,
                 lanePos_[i+1].first
            };
            painter->drawConvexPolygon(points, 4);
            //painter->drawLine(lanePos_[i].first.getX(), -lanePos_[i].first.getY(), lanePos_[i].second.getX(), -lanePos_[i].second.getY());
        }

        painter->drawImage(imgRect_.toRect(), image_);
    }
}
