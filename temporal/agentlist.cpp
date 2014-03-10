#include "agentlist.h"

AgentList::AgentList() {
    deepDeleted_ = false;
}

AgentList::~AgentList() {
    if (deepDeleted_) {
        for (QList<Agent*>::iterator i = list_.begin(); i != list_.end(); ++i) {
            delete *i;
        }
    }
}

AgentList::AgentList(QList<Agent*> list) {
    deepDeleted_ = false;
    list_ = QList<Agent*>(list);
}

QList<Agent*>& AgentList::getList() {
    return list_;
}

int AgentList::size() {
    return list_.size();
}
