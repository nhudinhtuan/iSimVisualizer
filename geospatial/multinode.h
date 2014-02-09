#ifndef MULTINODE_H
#define MULTINODE_H

#include "node.h"

class MultiNode : public Node
{
public:
    MultiNode(unsigned long id, double xPos, double yPos, unsigned long aimsunId);
    ~MultiNode(){}
    iSimGUI::NodeType getType();
};

#endif // MULTINODE_H
