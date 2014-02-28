#ifndef G_DRIVER_H
#define G_DRIVER_H

#include "temporal/g_agent.h"
#include "temporal/driver.h"

class G_Driver : public G_Agent
{
public:
    G_Driver(QGraphicsItem *parent, Driver *model, PreferenceManager *preferenceManager);
    Agent* getModel();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    Driver* model_;
};
Q_DECLARE_METATYPE(G_Driver*)

#endif // G_DRIVER_H
