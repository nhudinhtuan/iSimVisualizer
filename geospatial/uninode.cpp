#include "uninode.h"

UniNode::UniNode(unsigned long id, QPointF pos, unsigned long aimsunId) :
    Node(id, pos, aimsunId) {
}

iSimGUI::NodeType UniNode::getType() {
    return iSimGUI::UNI_NODE;
}

QString UniNode::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", 1";
    result += ", " + QString::number(aimsunId_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ")";
    return result;
}
