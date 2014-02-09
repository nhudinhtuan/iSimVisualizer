#include "busdriver.h"

BusDriver::BusDriver(unsigned long id, unsigned int tick, long xPos, long yPos, double angle, double length, double width,
          int passenger, unsigned long realArrivalTime, int dwellTime, QString busLineID):
    Driver(id, tick, xPos, yPos, angle, length, width, passenger) {
    realArrivalTime_ = realArrivalTime;
    dwellTime_ = dwellTime;
    busLineID_ = busLineID;
}

BusDriver::BusDriver(Agent const* other) {
    copy(other);
}

BusDriver::BusDriver() {
    realArrivalTime_ = 0;
    dwellTime_ = 0;
    busLineID_ = "";
}

void BusDriver::copy(Agent const* other) {
    Driver::copy(other);
    if (other->getType() == this->getType()) {
        BusDriver const * temp = (BusDriver const *)other;
        realArrivalTime_ = temp->getRealArrivalTime();
        dwellTime_ = temp->getDwellTime();
        busLineID_ = temp->getBusLineID();
    }
}
