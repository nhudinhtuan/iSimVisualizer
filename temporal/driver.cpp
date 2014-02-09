#include "driver.h"

Driver::Driver(unsigned long id, unsigned int tick, long xPos, long yPos, double angle, double length,
       double width, int passenger, int mandatory, QString info)
    :Agent(id, tick, xPos, yPos, angle), length_(length), width_(width), passenger_(passenger) {
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

Driver::Driver(Agent const* other) {
    copy(other);
}

void Driver::copy(Agent const* other) {
    Agent::copy(other);
    if (other->getType() == iSimGUI::AGENT_DRIVER || other->getType() == iSimGUI::AGENT_BUS) {
        Driver const * temp = (Driver const *)other;
        length_ = temp->getLength();
        width_ = temp->getWidth();
        passenger_ = temp->getPassenger();
        mandatory_ = temp->getMandatory();
        info_ = temp->getInfo();
    }
}
