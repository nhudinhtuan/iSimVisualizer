#include "busdriver.h"

BusDriver::BusDriver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length, double width,
          int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID):
    Driver(id, tick, pos, angle, length, width, passenger) {
    realArrivalTime_ = realArrivalTime;
    dwellTime_ = dwellTime;
    busLineID_ = busLineID;
}

BusDriver::BusDriver() {
    realArrivalTime_ = 0;
    dwellTime_ = 0;
    busLineID_ = "";
}

void BusDriver::update(BusDriver* data) {
    Driver::update(data);
    realArrivalTime_ = data->realArrivalTime_;
    dwellTime_ = data->dwellTime_;
    busLineID_ = data->busLineID_;
}
