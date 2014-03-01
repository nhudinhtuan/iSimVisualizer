#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QDebug>

namespace iSimGUI {
    enum PreferenceType {
        PREF_UNINODE = 0,
        PREF_MULTINODE = 1,
        PREF_SEGMENT = 2,
        PREF_LANE = 3,
        PREF_LANE_CONNECTOR = 4,
        PREF_BUSSTOP = 5,
        PREF_MICRO = 6,
    };

    const QString ICON_DIRECTORY = ":/agent_icons/";
    const int N_DRIVER_ICON = 4;
    const QString ICON_DRIVERS[] = {"car-red.png", "car-green.png", "car-yellow.png", "sport-car.png"};
    const int N_BUS_ICON = 1;
    const QString ICON_BUSES[] = {"bus-yellow.png"};
    const int N_PEDESTRIAN_ICON = 1;
    const QString ICON_PEDESTRIANS[] = {"pedes-white.png"};

    const int N_NODE_EXTRA_INFO = 4;
    const QString NODE_EXTRA_INFO[] = {"None", "Node Id", "location", "Aimsun Id"};
    const int N_SEGMENT_EXTRA_INFO = 3;
    const QString SEGMENT_EXTRA_INFO[] = {"None", "Segment Id", "Aimsun Id"};
    const int N_LANE_EXTRA_INFO = 2;
    const QString LANE_EXTRA_INFO[] = {"None", "Lane Index"};
}

class RangeData
{
public:
    RangeData(double lowerBound, double upperBound, QColor &color) :
        lowerBound(lowerBound), upperBound(upperBound), color(color) {}

    QColor& getColor() { return color; }
    double getLowerBound() { return lowerBound; }
    double getUpperBound() { return upperBound; }

    bool isInRange(double value) { return (value>=lowerBound && value<upperBound); }

private:
    double lowerBound;
    double upperBound;
    QColor &color;
};


class PreferenceManager : public QObject
{
    Q_OBJECT
public:
    explicit PreferenceManager(QObject *parent = 0);
    ~PreferenceManager();

    bool isUninodeDisplayed() { return isUniNodeShown_;}
    bool isMultinodeDisplayed() { return isMultiNodeShown_;}
    bool isSegmentDisplayed() { return isSegmentShown_;}
    bool isLaneDisplayed() { return isLaneShown_;}
    bool isLaneConnectorDisplayed() { return isLaneConnectorShown_;}
    bool isBusStopDisplayed() { return isBusStopShown_;}
    bool isMicroscopicDisplayed() {return isMicroscopicShown_;}

    QColor& getBgColor() { return bgColor_;}
    void setBgColor(QColor);

    QColor& getUninodeColor() { return uninodeColor_;}
    QColor& getMultinodeColor() { return multinodeColor_;}
    QColor& getSegmentColor() { return segmentColor_;}
    QColor& getLaneColor() { return laneColor_;}
    QColor& getLaneConnectorColor() { return laneConnectorColor_;}

    int getUninodeThreshold() { return uninodeThreshold_;}
    int getMultinodeThreshold() { return multinodeThreshold_;}
    int getSegmentThreshold() { return segmentThreshold_;}
    int getLaneThreshold() { return laneThreshold_;}
    int getLaneConnectorThreshold() { return laneConnectorThreshold_;}
    int getBusstopThreshold() { return busstopThreshold_;}
    int getMicroscopicThreshold() { return microscopicThreshold_;}

    QString getDriverIcon() { return iSimGUI::ICON_DRIVERS[driverIcon_];}
    int getDriverIconIndex() { return driverIcon_;}
    QString getBusIcon() { return iSimGUI::ICON_BUSES[busIcon_];}
    int getBusIconIndex() { return busIcon_;}
    QString getPedestrianIcon() { return iSimGUI::ICON_PEDESTRIANS[pedestrianIcon_];}
    int getPedestrianIconIndex() { return pedestrianIcon_;}

    int getUninodeExtraInfo() { return uninodeExtraInfo_;}
    int getMultinodeExtraInfo() { return multinodeExtraInfo_;}
    int getSegmentExtraInfo() { return segmentExtraInfo_;}
    int getLaneExtraInfo() { return laneExtraInfo_;}

    void setDriverIcon(int val);
    void setBusIcon(int val);
    void setPedestrianIcon(int val);

    void updateShownAttributes(iSimGUI::PreferenceType type, bool value);
    void updateColorAttributes(iSimGUI::PreferenceType  type, QColor color);
    void updateThresholdAttributes(iSimGUI::PreferenceType  type, int value);
    void updateExtraInfoAttributes(iSimGUI::PreferenceType  type, int value);

signals:
    void updateBgColor();
    void updateMapViewAttr();
    void updateMicroData();
    void updateAgents();

public slots:
    void initDisplayed();
    void initColor();
    void initThreshold();
    void initExtraInfo();
    void initAgentIcon();


private:

    QSettings *settings_;

    bool isUniNodeShown_;
    bool isMultiNodeShown_;
    bool isSegmentShown_;
    bool isLaneShown_;
    bool isLaneConnectorShown_;
    bool isBusStopShown_;
    bool isMicroscopicShown_;

    QColor bgColor_;
    QColor uninodeColor_;
    QColor multinodeColor_;
    QColor segmentColor_;
    QColor laneColor_;
    QColor laneConnectorColor_;

    int uninodeThreshold_;
    int multinodeThreshold_;
    int segmentThreshold_;
    int laneThreshold_;
    int laneConnectorThreshold_;
    int busstopThreshold_;
    int microscopicThreshold_;

    int uninodeExtraInfo_;
    int multinodeExtraInfo_;
    int segmentExtraInfo_;
    int laneExtraInfo_;

    int driverIcon_;
    int busIcon_;
    int pedestrianIcon_;
};

#endif // PREFERENCEMANAGER_H
