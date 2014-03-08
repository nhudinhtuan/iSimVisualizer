#ifndef MESOSCOPIC_H
#define MESOSCOPIC_H

class Mesoscopic
{
public:
    Mesoscopic(unsigned long segmentID, unsigned int tick, double speed, double flow, double density) :
        segmentID_(segmentID), tick_(tick), speed_(speed), flow_(flow), density_(density) {}
    Mesoscopic(): segmentID_(0), tick_(0), speed_(0), flow_(0), density_(0) {}
    Mesoscopic(Mesoscopic& another);
    void update(Mesoscopic& another);
    void setSegmentID(unsigned long id) { segmentID_ = id; }
    void setTick(unsigned int data) { tick_ = data; }
    void setSpeed(double value) { speed_ = value; }
    void setFlow(double value) { flow_ = value; }
    void setDensity(double value) {density_ = value; }
    unsigned long getSegmentID() { return segmentID_; }
    unsigned int getTick() { return tick_; }
    double getSpeed() { return speed_; }
    double getFlow() { return flow_; }
    double getDensity() { return density_; }
    double getValueByMode(int mode);

private:
    unsigned long segmentID_;
    unsigned int tick_;
    double speed_;
    double flow_;
    double density_;
};

#endif // MESOSCOPIC_H
