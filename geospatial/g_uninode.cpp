#include "g_uninode.h"

G_UniNode::G_UniNode(QGraphicsItem *parent, UniNode *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : G_Node(parent, model, preferenceManager, mapView){
}

void G_UniNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isUninodeDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getUninodeThreshold()) {
        if (isSelected()) {
            brush_.setColor(colorForSelected_);
        } else {
            brush_.setColor(preferenceManager_->getUninodeColor());
        }
        painter->fillPath(shape_, brush_);

        if (preferenceManager_->getUninodeExtraInfo()) {
            annotation_->setBrush(brush_);
            updateAnnotation();
            annotation_->setVisible(true);
        } else {
            annotation_->setVisible(false);
        }
    }
}

void G_UniNode::updateAnnotation() {
    switch (preferenceManager_->getUninodeExtraInfo()) {
        case 0: { // no annotation
            annotation_->setText("");
            break;
        }
        case 1: { // node ID
            annotation_->setText(QString::number(model_->getId()));
            break;
        }
        case 2: { // location
            QPointF& pos = model_->getPos();
            annotation_->setText(QString("(%1, %2)").arg(QString::number(pos.x())).arg(QString::number(-pos.y())));
            break;
        }
        case 3: { // aimsun ID
            annotation_->setText(QString::number(model_->getAimsunId()));
            break;
        }
    }
}
