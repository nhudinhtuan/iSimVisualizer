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

void BusDriver::update(BusDriver* data) {
    Agent::update(data);
    length_ = data->length_;
    width_ = data->width_;
    realArrivalTime_ = data->realArrivalTime_;
    dwellTime_ = data->dwellTime_;
    busLineID_ = data->busLineID_;
}
