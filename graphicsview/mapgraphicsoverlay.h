#ifndef MAPGRAPHICSOVERLAY_H
#define MAPGRAPHICSOVERLAY_H

#include <QPainter>
#include "preferencemanager.h"

class MapGraphicsOverlay
{
public:
    MapGraphicsOverlay(PreferenceManager *preferenceManager);
    ~MapGraphicsOverlay();
    void display(QPainter& painter);

    void updatePenColor();
    void updateTitle(int type); // 0: None, 1 : Mesoscopic, 2 : Microscopic, 3 : both
    void updateMesosColorRangeLabel();
    void clearMesosColorRangeLabel();
    void reset();

private:
    PreferenceManager *preferenceManager_;

    QPoint titlePos_;
    QString title_;
    QPen pen_;

    QList<QString> rangeTexts_;
    QList<QPoint> rangeTextPos_;
    QList<QPainterPath> rangeRects_;
    QList<QBrush> rangeBrushes_;
    bool showMesoscopicRangeLabel_;
};

#endif // MAPGRAPHICSOVERLAY_H
