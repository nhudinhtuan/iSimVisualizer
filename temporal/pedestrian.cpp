#include "pedestrian.h"

Pedestrian::Pedestrian(const Pedestrian& other)
    : Agent(other){

}

void Pedestrian::update(Pedestrian* pedestrian) {
    Agent::update(pedestrian);
}

QString Pedestrian::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(tick_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ", " + QString::number(angle_);
    result += ", 2";
    result += ", \'\')";
    return result;
}
