#include "g_multinode.h"

G_MultiNode::G_MultiNode(QGraphicsItem *parent, MultiNode *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : G_Node(parent, model, preferenceManager, mapView){
}

void G_MultiNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (preferenceManager_->isMultinodeDisplayed() && mapView_->getZoomFactor() >= preferenceManager_->getMultinodeThreshold()) {
        if (isSelected()) {
            brush_.setColor(colorForSelected_);
        } else {
            brush_.setColor(preferenceManager_->getMultinodeColor());
        }
        painter->fillPath(shape_, brush_);

        if (preferenceManager_->getMultinodeExtraInfo()) {
            annotation_->setBrush(brush_);
            updateAnnotation();
            annotation_->setVisible(true);
        } else {
            annotation_->setVisible(false);
        }
    } else {
        annotation_->setVisible(false);
    }
}

void G_MultiNode::updateAnnotation() {
    switch (preferenceManager_->getMultinodeExtraInfo()) {
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
