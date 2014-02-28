#ifndef AGENTLIST_H
#define AGENTLIST_H

#include <QList>
#include <QVariant>
#include <QDebug>
#include "temporal/agent.h"

class AgentList
{
public:
    AgentList();
    AgentList(QList<Agent*> list);
    QList<Agent*>& getList();
    int size();

private:
    QList<Agent*> list_;
};
Q_DECLARE_METATYPE(AgentList*)
#endif // AGENTLIST_H
