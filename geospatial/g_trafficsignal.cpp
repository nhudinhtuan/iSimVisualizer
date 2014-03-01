#include "g_trafficsignal.h"
#define ARROW_SIZE 200.0
#define PI 3.14

G_TrafficSignal::G_TrafficSignal(QGraphicsItem *parent, TrafficSignal *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView, TemporalIndex *temporalIndex)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView), temporalIndex_(temporalIndex), model_(model) {

    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::MiterJoin);
    pen_.setStyle(Qt::SolidLine);
    pen_.setWidth(40);
    brush_.setStyle(Qt::SolidPattern);
    createShape();
}

G_TrafficSignal::~G_TrafficSignal() {

}

void G_TrafficSignal::createShape() {
    QList<TrafficPhase*> phases = model_->getPhases();
    for (QList<TrafficPhase*>::iterator phaseIt = phases.begin(); phaseIt != phases.end(); ++phaseIt) {
        TrafficPhase* phase = *phaseIt;
        for (int i = 0; i < phase->lines.size(); i++) {
            QPair<QPointF, QPointF>& points = phase->lines[i];

            QPainterPath line;
            QPainterPath arrow;
            line.moveTo(points.first);
            // in the future , shall use centreCurvePath.quadTo()
            line.lineTo(points.second);

            // draw arrow head
            double angle = line.angleAtPercent(1)/57.3;
            QPointF arrowP1 = points.second - QPointF(sin(angle + PI / 3) * ARROW_SIZE,
                                            cos(angle + PI / 3) * ARROW_SIZE);
            QPointF arrowP2 = points.second - QPointF(sin(angle + PI - PI / 3) * ARROW_SIZE,
                                            cos(angle + PI - PI / 3) * ARROW_SIZE);
            arrow.moveTo(points.second);
            arrow.lineTo(arrowP1);
            arrow.lineTo(arrowP2);
            lines_[phase->name].append(line);
            arrows_[phase->name].append(arrow);
            shape_.addPath(line);
            shape_.addPath(arrow);
        }
    }
}

QRectF G_TrafficSignal::boundingRect() const {
    return shape_.boundingRect();
}

void G_TrafficSignal::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(pen_);

    if (preferenceManager_->isMicroscopicDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getMicroscopicThreshold()) {
        TrafficPhaseData* phaseData = temporalIndex_->getTrafficPhaseData(model_->getId());
        if (phaseData) {
            QVector<unsigned int>& colors = phaseData->colors;
            for (int i = 0; i < colors.size(); i++) {
                setColor(colors[i]);
                painter->drawPath(lines_[phaseData->name][i]);
                painter->fillPath(arrows_[phaseData->name][i], brush_);
            }
        }
    }
}

void G_TrafficSignal::setColor(unsigned int color) {
    switch(color) {
        case 1: pen_.setColor(QColor(200,10,10,200));
                brush_.setColor(QColor(200,10,10,200));
                break;
        case 2: pen_.setColor(QColor(200,150,10,200));
                brush_.setColor(QColor(200,150,10,200));
                break;
        case 3: pen_.setColor(QColor(10,200,10,200));
                brush_.setColor(QColor(10,200,10,200));
                break;
        default: break;
    }
}
