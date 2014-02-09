#include "uninode.h"

UniNode::UniNode(unsigned long id, double xPos, double yPos, unsigned long aimsunId) :
    Node(id, xPos, yPos, aimsunId) {
}

iSimGUI::NodeType UniNode::getType() {
    return iSimGUI::UNI_NODE;
}
