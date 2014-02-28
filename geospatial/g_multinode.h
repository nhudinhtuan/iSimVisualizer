#ifndef G_MULTINODE_H
#define G_MULTINODE_H

#include "geospatial/multinode.h"
#include "geospatial/g_node.h"

class G_MultiNode : public G_Node
{
public:
    G_MultiNode(QGraphicsItem *parent, MultiNode *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateAnnotation();
};
Q_DECLARE_METATYPE(G_MultiNode*)
#endif // G_MULTINODE_H
