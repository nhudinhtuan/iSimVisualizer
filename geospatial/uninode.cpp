#include "uninode.h"

const int UniNode::DB_ID = 1;

UniNode::UniNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId) {
}

iSimGUI::NodeType UniNode::getType() {
    return iSimGUI::UNI_NODE;
}

QString UniNode::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);
    result += ", " + QString::number(aimsunId_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ")";
    return result;
}
