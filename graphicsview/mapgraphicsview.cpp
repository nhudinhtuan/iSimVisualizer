#include "mapgraphicsview.h"
#include <QDebug>

MapGraphicsView::MapGraphicsView(QGraphicsScene *scene, QWidget *parent, PreferenceManager *preferenceManager):
    QGraphicsView(scene, parent), preferenceManager_(preferenceManager)
{
    setCacheMode(CacheBackground);

    setAutoFillBackground(false);
    setBackgroundBrush(QBrush(preferenceManager_->getBgColor(), Qt::SolidPattern));
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, false);
    setOptimizationFlags(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(ScrollHandDrag);
    setWindowTitle(tr("iSimGUI Map"));

    zoomFactor_ = 1.0;
    overlay_ = new MapGraphicsOverlay(preferenceManager_);
}

void MapGraphicsView::updateBackgroundColor() {
    overlay_->updatePenColor();
    setBackgroundBrush(QBrush(preferenceManager_->getBgColor(), Qt::SolidPattern));
}

void MapGraphicsView::updateOverlayTitle(int type) {
    overlay_->updateTitle(type);
}

void MapGraphicsView::reset() {
    overlay_->reset();
}

void MapGraphicsView::updateOverLayeRangeTitle() {
    overlay_->updateMesosColorRangeLabel();
}

MapGraphicsView::~MapGraphicsView() {
    delete overlay_;
}

QRectF MapGraphicsView::getGraphViewRect() {
    return QRectF(mapToScene(0,0), mapToScene(width(), height()));
}

QList<QGraphicsItem*> MapGraphicsView::getSeenItems() {
    return scene()->items(getGraphViewRect());
}

void MapGraphicsView::setZoomFactor(double factor) {
    zoomFactor_ = factor;
}

void MapGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    emit announceMousePointerPosition(QPointF(mapToScene(event->pos())).toPoint());
    QGraphicsView::mouseMoveEvent(event);
}

//scrolls the view and after the mouse is released, triggers the signal
void MapGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
//    emit announceMapviewScroll();
}

void MapGraphicsView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta()/8.0;
    double numSteps = numDegrees/15.0;
    double factor = pow(1.125, numSteps);
    zoomFactor_ = zoomFactor_*factor;
    scale(factor, factor);

    emit announceZoomFactorChange(zoomFactor_);
    emit announceMapviewScroll();
}

void MapGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QPainter p(viewport());
    overlay_->display(p);
}

