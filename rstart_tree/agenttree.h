#ifndef AGENTTREE_H
#define AGENTTREE_H

#include <QList>
#include <QPointF>
#include "RStarTree.h"
#include "../temporal/agent.h"

class AgentTree : public RStarTree<Agent*, 2, 8, 16>
{
public:
    // Insert an agent into the tree, based on the agent's position.
    void insert(Agent *agent);

    // Return an array of agents that are located inside the search rectangle.
    // box.edges[].first is the lower-left corner and box.edges[].second is the
    // upper-right corner.  box.edges[0] is the x- component and box.edges[1] is the
    // y- component.
    QList<Agent*> query(QPoint& lowerLeft, QPoint& upperRight);
};

// A visitor that simply collects the agent into an array, which was specified in the
// constructor.
struct Collecting_visitor
{
    const bool ContinueVisiting;
    QList<Agent*> &array;  // must be a reference.

    explicit Collecting_visitor(QList<Agent*> &array) :
            ContinueVisiting(true), array(array)
    {
    }

    // When called, the visitor saves the agent in <array>.
    bool operator()(AgentTree::Leaf *leaf) const
    {
        array.push_back(leaf->leaf);
        return true;
    }
};

#endif // AGENTTREE_H
