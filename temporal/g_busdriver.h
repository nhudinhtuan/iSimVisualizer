#ifndef G_BUSDRIVER_H
#define G_BUSDRIVER_H

#include "temporal/g_agent.h"

class G_BusDriver : public G_Agent
{
public:
    G_BusDriver(QGraphicsItem *parent, BusDriver *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_BUS; }
    void updateModel(Agent* model);
    unsigned long getModelId() { return model_.getID();}

    Agent* getModel() { return &model_;}
    QList<QTableWidgetItem*> buildPropertyTable();
private:
    void updateModel(BusDriver *model);
    BusDriver model_;
};
Q_DECLARE_METATYPE(G_BusDriver*)

#endif // G_BUSDRIVER_H
