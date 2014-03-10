#include "busdriver.h"

BusDriver::BusDriver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length, double width,
          int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID):
    Agent(id, tick, pos, angle) {
    length_ = length;
    width_ = width;
    passenger_ = passenger;
    realArrivalTime_ = realArrivalTime;
    dwellTime_ = dwellTime;
    busLineID_ = busLineID;
}

BusDriver::BusDriver() {
    realArrivalTime_ = 0;
    dwellTime_ = 0;
    busLineID_ = "";
    length_ = 0;
    width_ = 0;
    passenger_ = 0;
}

BusDriver::BusDriver(const BusDriver& other)
    : Agent(other){
    realArrivalTime_ = other.realArrivalTime_;
    dwellTime_ = other.dwellTime_;
    busLineID_ = other.busLineID_;
    length_ = other.length_;
    width_ = other.width_;
    passenger_ = other.passenger_;
}

void BusDriver::update(BusDriver* data) {
    Agent::update(data);
    length_ = data->length_;
    width_ = data->width_;
    realArrivalTime_ = data->realArrivalTime_;
    dwellTime_ = data->dwellTime_;
    busLineID_ = data->busLineID_;
}

QString BusDriver::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(tick_);
    result += ", " + QString::number(pos_.x());
    result += ", " + QString::number(-pos_.y());
    result += ", " + QString::number(angle_);
    result += ", " + QString::number(length_);
    result += ", " + QString::number(width_);
    result += ", " + QString::number(passenger_);
    result += ", " + QString::number(realArrivalTime_);
    result += ", " + QString::number(dwellTime_);
    result += ",\'" + busLineID_ + "\'";
    result += ")";
    return result;
}
