#include "multinode.h"

MultiNode::MultiNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId){

}


iSimGUI::NodeType MultiNode::getType() {
    return iSimGUI::MULTI_NODE;
}
