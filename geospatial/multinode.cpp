#include "multinode.h"

const int MultiNode::DB_ID = 2;

MultiNode::MultiNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId){

}

iSimGUI::NodeType MultiNode::getType() {
    return iSimGUI::MULTI_NODE;
}

QString MultiNode::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);;
    result += ", " + QString::number(aimsunId_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ")";
    return result;
}
