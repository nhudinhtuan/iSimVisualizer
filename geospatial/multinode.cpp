#include "multinode.h"

MultiNode::MultiNode(unsigned long id, double xPos, double yPos, unsigned long aimsunId) :
    Node(id, xPos, yPos, aimsunId){

}


iSimGUI::NodeType MultiNode::getType() {
    return iSimGUI::MULTI_NODE;
}
