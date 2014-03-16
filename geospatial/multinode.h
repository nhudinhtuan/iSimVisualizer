#ifndef MULTINODE_H
#define MULTINODE_H

#include "node.h"

class MultiNode : public Node
{
public:
    MultiNode(unsigned long id, QPointF pos, unsigned long aimsunId);
    ~MultiNode(){}
    iSimGUI::NodeType getType();
    QString sqlInsertValue() const ;
};

#endif // MULTINODE_H
