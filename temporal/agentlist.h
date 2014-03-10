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
    ~AgentList();
    AgentList(QList<Agent*> list);
    QList<Agent*>& getList();
    int size();
    void setDeepDelete() {deepDeleted_ = true;}

private:
    bool deepDeleted_;
    QList<Agent*> list_;
};
Q_DECLARE_METATYPE(AgentList*)
#endif // AGENTLIST_H
