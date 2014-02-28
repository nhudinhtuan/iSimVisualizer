#ifndef G_UNINODE_H
#define G_UNINODE_H
#include "geospatial/g_node.h"
#include "geospatial/uninode.h"

class G_UniNode : public G_Node
{
public:
    G_UniNode(QGraphicsItem *parent, UniNode *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateAnnotation();
};
Q_DECLARE_METATYPE(G_UniNode*)
#endif // G_UNINODE_H
