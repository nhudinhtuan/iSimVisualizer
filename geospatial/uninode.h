#ifndef UNINODE_H
#define UNINODE_H

#include "node.h"

class UniNode : public Node
{
public:
    UniNode(unsigned long id, QPointF pos, unsigned long aimsunId);
    ~UniNode(){}
    iSimGUI::NodeType getType();
    QString sqlInsertValue() const;
};

#endif // UNINODE_H
