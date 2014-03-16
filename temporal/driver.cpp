#include "driver.h"

Driver::Driver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length,
       double width, unsigned long currentSegment, int fwdSpeed, int fwdAccel, int mandatory, QString info)
    :Agent(id, tick, pos, angle) {
    length_ = length;
    width_ = width;
    currentSegment_ = currentSegment;
    fwdSpeed_ = fwdSpeed;
    fwdAccel_ = fwdAccel;
    mandatory_ = mandatory;
    info_ = info;
}

Driver::Driver() {
    length_ = 0;
    width_ = 0;
    currentSegment_ = 0;
    fwdSpeed_ = 0;
    fwdAccel_ = 0;
    mandatory_ = -1;
    info_ = "";
}

Driver::Driver(const Driver& other)
    : Agent(other){
    length_ = other.length_;
    width_ = other.width_;
    currentSegment_ = other.currentSegment_;
    fwdSpeed_ = other.fwdSpeed_;
    fwdAccel_ = other.fwdAccel_;
    mandatory_ = other.mandatory_;
    info_ = other.info_;
}

void Driver::update(Driver* other)
{
    Agent::update(other);
    length_ = other->length_;
    width_ = other->width_;
    fwdSpeed_ = other->fwdSpeed_;
    fwdAccel_ = other->fwdAccel_;
    mandatory_ = other->mandatory_;
    currentSegment_ = other->currentSegment_;
    info_ = other->info_;
}

QString Driver::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(tick_);
    result += ", " + QString::number(pos_.x(), 'f', 2);
    result += ", " + QString::number(-pos_.y(), 'f', 2);
    result += ", " + QString::number(angle_);
    // attributes
    result += ", 0";
    result += ", \'" + QString::number(length_);
    result += ":" + QString::number(width_);
    result += ":" + QString::number(mandatory_);
    result += ":" + QString::number(currentSegment_);
    result += ":" + QString::number(fwdSpeed_);
    result += ":" + QString::number(fwdAccel_);
    result += ":" + info_ + "\'";
    result += ")";
    return result;
}
