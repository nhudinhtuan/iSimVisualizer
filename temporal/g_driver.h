#ifndef G_DRIVER_H
#define G_DRIVER_H

#include "temporal/g_agent.h"

class G_Driver : public G_Agent
{
public:
    G_Driver(QGraphicsItem *parent, Driver *model, PreferenceManager *preferenceManager, MapGraphicsView *mapView);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    iSimGUI::AgentType getType() const { return iSimGUI::AGENT_DRIVER; }
    void updateModel(Agent* model);
    unsigned long getModelId() { return model_.getID();}

    Agent* getModel() { return &model_;}
    QList<QTableWidgetItem*> buildPropertyTable();
protected:
    void updateModel(Driver *model);
    Driver model_;
};
Q_DECLARE_METATYPE(G_Driver*)

#endif // G_DRIVER_H
