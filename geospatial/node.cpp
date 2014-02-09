#include "node.h"

Node::Node(unsigned long id, double xPos, double yPos, unsigned long aimsunId) {
    id_ = id;
    aimsunId_ = aimsunId;
    xPos_ = xPos;
    yPos_ = yPos;
}
