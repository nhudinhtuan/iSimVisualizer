#include "agenttree.h"

AgentTree::~AgentTree() {
    for (QList<Agent*>::iterator i = agents_.begin(); i != agents_.end(); ++i)
        delete *i;
}

void AgentTree::insert(Agent *agent) {
    // The agent has no width nor length.  So the lower-left corner equals to the
    // upper-right corner and is equal to the agent's position.
    BoundingBox box;
    box.edges[0].first = box.edges[0].second = agent->getPos().x();
    box.edges[1].first = box.edges[1].second = -agent->getPos().y();
    // Insert an agent into the tree, based on the bounding-box that encloses the agent.
    Insert(agent, box);
    agents_.append(agent);
}

QList<Agent*> AgentTree::query(QPoint& lowerLeft, QPoint& upperRight) {
    BoundingBox box;
    box.edges[0].first = lowerLeft.x();
    box.edges[1].first = lowerLeft.y();
    box.edges[0].second = upperRight.x();
    box.edges[1].second = upperRight.y();

    // AgentTree::AcceptEnclosing functor will call the visitor if the agent is enclosed
    // in <box>.  When called, the visitor saves the agent in <result>.  Therefore, when
    // Query() returns, <result> should contain agents that are located inside <box>.
    QList<Agent*> result;
    this->Query(AcceptEnclosing(box), Collecting_visitor(result));
    // Need to remove the constness of <this> because Query() was not implemented as a
    // const method.
    return result;
}

