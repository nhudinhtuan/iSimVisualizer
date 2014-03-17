#ifndef MULTINODE_H
#define MULTINODE_H

#include "node.h"

class MultiNode : public Node
{
public:
    static const int DB_ID;
    MultiNode(unsigned long id, QPointF pos, unsigned long aimsunId);
    ~MultiNode(){}
    iSimGUI::NodeType getType();
    QString sqlInsertValue() const ;
};

#endif // MULTINODE_H
