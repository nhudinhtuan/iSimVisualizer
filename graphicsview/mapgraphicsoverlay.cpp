#include "mapgraphicsoverlay.h"

MapGraphicsOverlay::MapGraphicsOverlay(PreferenceManager *preferenceManager)
    : preferenceManager_ (preferenceManager)
{
    titlePos_.setX(10);
    titlePos_.setY(20);
    updatePenColor();
    updateTitle(0);
    showMesoscopicRangeLabel_ = false;
}

MapGraphicsOverlay::~MapGraphicsOverlay() {

}

void MapGraphicsOverlay::reset() {
    updateTitle(0);
    showMesoscopicRangeLabel_ = false;
    clearMesosColorRangeLabel();
}

void MapGraphicsOverlay::updateMesosColorRangeLabel() {
    clearMesosColorRangeLabel();
    int mode = preferenceManager_->getMesoscopicMode();
    QString rangeLabel;
    QList<RangeData*> *list;
    switch(mode) {
        case 0: list = &preferenceManager_->getMesoscopicDensityColorRanges();
                rangeLabel = "Density Ranges";
                break;
        case 1: list = &preferenceManager_->getMesoscopicFlowColorRanges();
                rangeLabel = "Flow Ranges";
                break;
        case 2: list = &preferenceManager_->getMesoscopicSpeedColorRanges();
                rangeLabel = "Speed Ranges";
                break;
        default: list = &preferenceManager_->getMesoscopicSpeedColorRanges();
                rangeLabel = "Speed Ranges";
                break;
    }

    rangeTextPos_.push_back(QPoint(10, 40));
    rangeTexts_.push_back(rangeLabel);
    int index = 0;
    for (QList<RangeData*>::iterator i = list->begin(); i != list->end(); ++i) {
        RangeData *range = *i;
        QPainterPath rectangle;
        rectangle.addRect(10, 30*index + 50, 25, 25);
        rangeRects_.push_back(rectangle);
        QBrush brush(range->getColor());
        rangeBrushes_.push_back(brush);
        QPoint point(60, 30*index + 67);
        rangeTextPos_.push_back(point);
        QString text = QString::number(range->getLow(), 'f', 3) + " - " + QString::number(range->getHigh(), 'f', 3);
        rangeTexts_.push_back(text);
        index++;
    }
}

void MapGraphicsOverlay::clearMesosColorRangeLabel() {
    rangeTexts_.clear();
    rangeTextPos_.clear();
    rangeRects_.clear();
    rangeBrushes_.clear();
}

void MapGraphicsOverlay::display(QPainter& painter) {
    painter.setPen(pen_);
    painter.drawText(titlePos_, title_);

    if (showMesoscopicRangeLabel_) {
        for (int i = 0; i < rangeTexts_.size(); i++) {
            painter.drawText(rangeTextPos_[i], rangeTexts_[i]);
        }
        for (int i = 0; i < rangeRects_.size(); i++) {
            painter.fillPath(rangeRects_[i], rangeBrushes_[i]);
        }
    }
}

void MapGraphicsOverlay::updatePenColor() {
    pen_.setColor(preferenceManager_->getInvertBgColor());
}

void MapGraphicsOverlay::updateTitle(int type) {
    switch (type) {
        case 0: title_ = "";
                showMesoscopicRangeLabel_ = false;
                break;
        case 1: title_ = "MESOSCOPIC";
                showMesoscopicRangeLabel_ = true;
                updateMesosColorRangeLabel();
                break;
        case 2: title_ = "MICROSCOPIC";
                showMesoscopicRangeLabel_ = false;
                break;
        case 3: title_ = "MESOSCOPIC & MICROSCOPIC";
                showMesoscopicRangeLabel_ = true;
                updateMesosColorRangeLabel();
                break;
        default:
            break;
    }
}
