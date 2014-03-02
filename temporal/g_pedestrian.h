#ifndef G_PEDESTRIAN_H
#define G_PEDESTRIAN_H

#include "temporal/g_agent.h"
#include "temporal/pedestrian.h"

class G_Pedestrian : public G_Agent
{
public:
    G_Pedestrian(QGraphicsItem *parent, Pedestrian *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_PEDESTRIAN; }
    void updateModel(Agent* model);
    unsigned long getModelId() { return model_.getID();}

protected:
    void updateModel(Pedestrian *model);
    Pedestrian model_;
};
Q_DECLARE_METATYPE(G_Pedestrian*)
#endif // G_PEDESTRIAN_H
