#ifndef G_BUSDRIVER_H
#define G_BUSDRIVER_H

#include "temporal/g_agent.h"
#include "temporal/busdriver.h"

class G_BusDriver : public G_Agent
{
public:
    G_BusDriver(QGraphicsItem *parent, BusDriver *model, PreferenceManager *preferenceManager);
    Agent* getModel();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    BusDriver* model_;
};
Q_DECLARE_METATYPE(G_BusDriver*)

#endif // G_BUSDRIVER_H
