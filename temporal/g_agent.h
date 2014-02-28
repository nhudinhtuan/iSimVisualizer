#ifndef G_AGENT_H
#define G_AGENT_H

#include <QVariant>
#include <QObject>
#include <QGraphicsObject>
#include <QList>
#include "temporal/agent.h"
#include "preferencemanager.h"
#include "graphicsview/mapgraphicsview.h"

class G_Agent : public QGraphicsObject
{
    Q_OBJECT
public:
    G_Agent(QGraphicsItem *parent, PreferenceManager *preferenceManager);
    virtual ~G_Agent();
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
    virtual Agent* getModel() = 0;

protected:
    PreferenceManager *preferenceManager_;
    QImage image_;
    QRectF rect_;
    QBrush brush_;
    QPen pen_;
};
Q_DECLARE_METATYPE(G_Agent*)
#endif // G_AGENT_H
