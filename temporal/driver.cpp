#include "driver.h"

Driver::Driver(unsigned long id, unsigned int tick, QPointF pos, double angle, double length,
       double width, int passenger, int mandatory, QString info)
    :Agent(id, tick, pos, angle), length_(length), width_(width), passenger_(passenger) {
    mandatory_ = mandatory;
    info_ = info;
}

Driver::Driver() {
    length_ = 0;
    width_ = 0;
    passenger_ = 0;
    mandatory_ = -1;
    info_ = "";
}
