#ifndef NODE_H
#define NODE_H

#include <QPointF>

namespace iSimGUI{
    enum NodeType {UNI_NODE = 0, MULTI_NODE = 1};
}

class Node
{
public:
    Node(unsigned long id, QPointF pos, unsigned long aimsunId);
    virtual ~Node() {}
    unsigned long getId() {return id_;}
    unsigned long getAimsunId() {return aimsunId_;}
    QPointF& getPos() {return pos_;}
    virtual iSimGUI::NodeType getType() = 0;

protected:
    unsigned long id_;
    unsigned long aimsunId_;
    QPointF pos_;
};

#endif // NODE_H
