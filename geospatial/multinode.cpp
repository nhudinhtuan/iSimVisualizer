#include "multinode.h"

MultiNode::MultiNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId){

}


iSimGUI::NodeType MultiNode::getType() {
    return iSimGUI::MULTI_NODE;
}

QString MultiNode::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", 2";
    result += ", " + QString::number(aimsunId_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ")";
    return result;
}
