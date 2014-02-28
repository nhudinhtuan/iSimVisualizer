#ifndef G_NODE_H
#define G_NODE_H

#include <QVariant>
#include <QGraphicsObject>
#include <QBrush>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <QDebug>
#include "node.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"

class G_Node : public QGraphicsObject
{
    Q_OBJECT

public:
    G_Node(QGraphicsItem *parent, Node *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    virtual ~G_Node();
    QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    unsigned long getModelId() { return model_->getId();}
    iSimGUI::NodeType getNodeType() { return model_->getType();}
    Node* getModel() { return model_;}
    virtual void updateAnnotation() = 0;

protected:
    PreferenceManager *preferenceManager_;
    MapGraphicsView *mapView_;
    Node *model_;
    QBrush brush_;
    QPainterPath shape_;
    QColor colorForSelected_;
    QGraphicsSimpleTextItem *annotation_;
};

Q_DECLARE_METATYPE(G_Node*)
#endif // G_NODE_H
