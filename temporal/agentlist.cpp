#include "agentlist.h"

AgentList::AgentList() {

}

AgentList::AgentList(QList<Agent*> list) {
    list_ = QList<Agent*>(list);
}

QList<Agent*>& AgentList::getList() {
    return list_;
}

int AgentList::size() {
    return list_.size();
}
