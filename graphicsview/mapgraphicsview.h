#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>
#include <math.h>
#include "preferencemanager.h"
#include "mapgraphicsoverlay.h"

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QGraphicsScene *scene, QWidget *parent, PreferenceManager *preferenceManager);
    virtual ~MapGraphicsView();

    double getZoomFactor() {return zoomFactor_; }
    void setZoomFactor(double factor);
    QRectF getGraphViewRect();
    QList<QGraphicsItem*> getSeenItems();

    void updateOverlayTitle(int type);
    void reset();
    void updateOverLayeRangeTitle();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

private:

public slots:
    void updateBackgroundColor();

signals:
    void announceZoomFactorChange(double zoomFactor);
    void announceMousePointerPosition(QPoint point);
    void announceMapviewScroll();

private:
    PreferenceManager *preferenceManager_;
    MapGraphicsOverlay *overlay_;
    double zoomFactor_;
};

#endif // MAPGRAPHICSVIEW_H
