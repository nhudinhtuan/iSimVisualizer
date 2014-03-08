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
        PREF_MESOS = 7
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
    RangeData() {
        low_ = 0;
        high_ = 0;
        color_ = Qt::black;
    }
    RangeData(double low, double high, QColor color) :
        low_(low), high_(high), color_(color) {}
    RangeData(const RangeData& other) {
        low_ = other.low_;
        high_ = other.high_;
        color_ = other.color_;
    }

    QColor& getColor() { return color_; }
    double getLow() { return low_; }
    void setLow(double value) { low_ = value;}
    double getHigh() { return high_; }
    void setHigh(double value) { high_ = value;}
    bool isInRange(double value) { return (value>=low_ && value<high_); }
    void setColor(QColor color) { color_ = color;}

private:
    double low_;
    double high_;
    QColor color_;
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
    bool isMesoscopicDisplayed() {return isMesoscopicShown_;}

    QColor& getBgColor() { return bgColor_;}
    QColor getInvertBgColor();
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

    QString& getDBHost() { return dbHost_;}
    int getDBPort() { return dbPort_;}
    QString& getDBUsername() { return dbUser_;}
    QString& getDBPass() { return dbPass_;}
    QString& getDBName() { return dbName_;}

    int getMesoscopicMode() { return mesoscopicMode_;}
    QList<RangeData*>& getMesoscopicDensityColorRanges() { return mesoscopicDensityColorRangeList_;}
    QList<RangeData*>& getMesoscopicFlowColorRanges() { return mesoscopicFlowColorRangeList_;}
    QList<RangeData*>& getMesoscopicSpeedColorRanges() { return mesoscopicSpeedColorRangeList_;}
    void setMesoscopicMode(int mode);

    void setDBInfo(QString host, int port, QString username, QString password, QString dbName);

    void setDriverIcon(int val);
    void setBusIcon(int val);
    void setPedestrianIcon(int val);

    void updateShownAttributes(iSimGUI::PreferenceType type, bool value);
    void updateColorAttributes(iSimGUI::PreferenceType  type, QColor color);
    void updateThresholdAttributes(iSimGUI::PreferenceType  type, int value);
    void updateExtraInfoAttributes(iSimGUI::PreferenceType  type, int value);

    void updateMesosColorRange(int mode, QColor color, int index);
    void updateMesosLowValue(int mode, double value, int index);
    void updateMesosHighValue(int mode, double value, int index);
    RangeData* addNewMesosColorRange(int mode);
    void removeMesosColorRange(int mode, int index);

    QColor& getMesosColorByRange(double value);
    double getMesoscopicDensityPlotScale();
    double getMesoscopicFlowPlotScale();
    double getMesoscopicSpeedPlotScale();

signals:
    void updateBgColor();
    void updateMapViewAttr();
    void updateMesosData();
    void updateMicroData();
    void updateAgents();
    void updateDBConf();
    void updateMesosOverlay();

public slots:
    void initDisplayed();
    void initColor();
    void initThreshold();
    void initExtraInfo();
    void initAgentIcon();
    void initDBIConf();
    void initMesoscopicColorRanges();

private:

    QSettings *settings_;

    bool isUniNodeShown_;
    bool isMultiNodeShown_;
    bool isSegmentShown_;
    bool isLaneShown_;
    bool isLaneConnectorShown_;
    bool isBusStopShown_;
    bool isMicroscopicShown_;
    bool isMesoscopicShown_;

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

    QString dbHost_;
    int dbPort_;
    QString dbUser_;
    QString dbPass_;
    QString dbName_;

    int mesoscopicMode_;
    QList<RangeData*> mesoscopicDensityColorRangeList_;
    QList<RangeData*> mesoscopicFlowColorRangeList_;
    QList<RangeData*> mesoscopicSpeedColorRangeList_;
};

#endif // PREFERENCEMANAGER_H
