#ifndef NODE_H
#define NODE_H

namespace iSimGUI{
    enum NodeType {UNI_NODE = 0, MULTI_NODE = 1};
}

class Node
{
public:
    Node(unsigned long id, double xPos, double yPos, unsigned long aimsunId);
    ~Node() {}
    unsigned long getId() {return id_;}
    unsigned long getAimSunId() {return aimsunId_;}
    double getXPos() {return xPos_;}
    double getYPos() {return yPos_;}
    virtual iSimGUI::NodeType getType() = 0;

protected:
    unsigned long id_;
    unsigned long aimsunId_;
    double xPos_;
    double yPos_;
};

#endif // NODE_H
