#include "uninode.h"

UniNode::UniNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId) {
}

iSimGUI::NodeType UniNode::getType() {
    return iSimGUI::UNI_NODE;
}
