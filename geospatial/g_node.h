#ifndef G_NODE_H
#define G_NODE_H

#include <QGraphicsObject>
#include <QBrush>
#include <QPainterPath>
#include <QColor>
#include <QPainter>
#include "node.h"
#include "preferencemanager.h"

class G_Node : public QGraphicsObject
{
    Q_OBJECT

public:
    G_Node(QGraphicsItem *parent, Node *model, PreferenceManager *preferenceManager);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    PreferenceManager *preferenceManager_;
    Node *model_;
    QBrush brush_;
    QPainterPath shape_;
    QColor colorForSelected_;
};


#endif // G_NODE_H
