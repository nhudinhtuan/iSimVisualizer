#ifndef G_PEDESTRIAN_H
#define G_PEDESTRIAN_H

#include "temporal/g_agent.h"
#include "temporal/pedestrian.h"

class G_Pedestrian : public G_Agent
{
public:
    G_Pedestrian(QGraphicsItem *parent, Pedestrian *model, PreferenceManager *preferenceManager);
    Agent* getModel();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    Pedestrian* model_;
};
Q_DECLARE_METATYPE(G_Pedestrian*)
#endif // G_PEDESTRIAN_H
