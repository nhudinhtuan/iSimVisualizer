#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <math.h>
#include "preferencemanager.h"

class MapGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapGraphicsView(QGraphicsScene *scene, QWidget *parent, PreferenceManager *preferenceManager);
    virtual ~MapGraphicsView();

    double getZoomFactor() {return zoomFactor_; }
    void setZoomFactor(double factor);
    QRectF getGraphViewRect();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

private:

public slots:
    void myUpdateView();
    void focusOnItem(unsigned long id);

signals:
    void announceZoomFactorChange(double zoomFactor);
    void announceMousePointerPosition(QPoint point);
    void announceMapviewScroll();

private:
    PreferenceManager *preferenceManager_;
    double zoomFactor_;
};

#endif // MAPGRAPHICSVIEW_H
