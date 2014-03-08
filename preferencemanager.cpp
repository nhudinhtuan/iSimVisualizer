#include "preferencemanager.h"

namespace iSimGUI {

const QString UNINODE_DISPLAY_SETTING = "uniNodes/isDisplayed";
const bool DEFAULT_UNINODES_IS_DISPLAYED = true;
const QString MULTINODE_DISPLAY_SETTING = "mulNodes/isDisplayed";
const bool DEFAULT_MULTINODES_IS_DISPLAYED = true;
const QString SEGMENT_DISPLAY_SETTING = "segment/isDisplayed";
const bool DEFAULT_SEGMENTS_IS_DISPLAYED = true;
const QString LANE_DISPLAY_SETTING = "lanes/isDisplayed";
const bool DEFAULT_LANES_IS_DISPLAYED = true;
const QString LANE_CONNECTORS_DISPLAY_SETTING = "laneConnectors/isDisplayed";
const bool DEFAULT_LANE_CONNECTORS_IS_DISPLAYED = false;
const QString BUSSTOP_DISPLAY_SETTING = "busStop/isDisplayed";
const bool DEFAULT_BUS_STOP_IS_DISPLAYED = true;
const QString AGENT_DISPLAY_SETTING = "agent/isDisplayed";
const bool DEFAULT_AGENT_IS_DISPLAYED = true;

const QString MICROSCOPIC_DISPLAY_SETTING = "microscopic/isDisplayed";
const bool DEFAULT_MICROSCOPIC_IS_DISPLAYED = true;

const QString MESOSCOPIC_DISPLAY_SETTING = "mesoscopic/isDisplayed";
const bool DEFAULT_MESOSCOPIC_IS_DISPLAYED = true;

const QString MAP_COLOR_SETTING = "map/backgroundColor";
const QString DEFAULT_MAP_BG_COLOR = "black";

const QString UNINODE_COLOR_SETTING = "uniNodes/color";
const QString DEFAULT_UNINODE_COLOR = "gray";
const QString MULTINODE_COLOR_SETTING = "mulNodes/color";
const QString DEFAULT_MULTINODE_COLOR = "red";
const QString SEGMENT_COLOR_SETTING = "segment/color";
const QString DEFAULT_SEGMENT_COLOR = "blue";
const QString LANE_COLOR_SETTING = "lanes/color";
const QString DEFAULT_LANE_COLOR = "black";
const QString LANE_CONNECTOR_COLOR_SETTING = "laneConnector/color";
const QString DEFAULT_LANE_CONNECTOR_COLOR = "black";

const QString UNINODE_THRESHOLD_SETTING = "uniNodes/threshold";
const int DEFAULT_UNINODE_THRESHOLD = 10;
const QString MULTINODE_THRESHOLD_SETTING = "mulNodes/threshold";
const int DEFAULT_MULTINODE_THRESHOLD = 10;
const QString SEGMENT_THRESHOLD_SETTING = "segments/threshold";
const int DEFAULT_SEGMENT_THRESHOLD = 10;
const QString LANE_THRESHOLD_SETTING = "lanes/threshold";
const int DEFAULT_LANE_THRESHOLD = 10;
const QString LANE_CONNECTOR_THRESHOLD_SETTING = "laneConnectors/threshold";
const int DEFAULT_LANE_CONNECTOR_THRESHOLD = 15;
const QString CROSSING_THRESHOLD_SETTING = "crossing/threshold";
const int DEFAULT_CROSSING_THRESHOLD = 15;
const QString BUSSTOP_THRESHOLD_SETTING = "busStop/threshold";
const int DEFAULT_BUSSTOP_THRESHOLD = 10;

const QString UNINODE_EXTRA_INFO_SETTING = "uniNodes/extrainfo";
const int DEFAULT_UNINODE_EXTRA_INFO = 0;
const QString MULTINODE_EXTRA_INFO_SETTING = "mulNodes/extrainfo";
const int DEFAULT_MULTINODE_EXTRA_INFO = 0;
const QString SEGMENT_EXTRA_INFO_SETTING = "segments/extrainfo";
const int DEFAULT_SEGMENT_EXTRA_INFO = 0;
const QString LANE_EXTRA_INFO_SETTING = "lanes/extrainfo";
const int DEFAULT_LANE_EXTRA_INFO = 0;

const QString MICROSCOPIC_THRESHOLD_SETTING = "microscopic/threshold";
const int DEFAULT_MICROSCOPIC_THRESHOLD = 15;

const QString DRIVER_ICON_SETTING = "driver/icon";
const QString BUS_ICON_SETTING = "bus/icon";
const QString PEDESTRIAN_ICON_SETTING = "pedestrian/icon";

const QString DB_HOST_SETTING = "db/host";
const QString DEFAULT_DB_HOST = "localhost";
const QString DB_PORT_SETTING = "db/port";
const int DEFAULT_DB_PORT = 5432;
const QString DB_USERNAME_SETTING = "db/username";
const QString DEFAULT_DB_USERNAME = "postgres";
const QString DB_PASSWORD_SETTING = "db/password";
const QString DEFAULT_DB_PASSWORD = "";
const QString DB_NAME_SETTING = "db/name";
const QString DEFAULT_DB_NAME = "";


const QString MESOSCOPIC_MODE_SETTING = "mesoscopic/mode";
const int DEFAULT_MESOSCOPIC_MODE = 0;
const int DEFAULT_MESOSCOPIC_COLOR_ALPHA = 128;
const QString MESOSCOPIC_DENSITY_SETTING = "mesoscopic/density";
const int DEFAULT_MESOSCOPIC_DENSITY_RANGE_COUNT = 4;
const QString DEFAULT_MESOSCOPIC_DENSITY_RANGE_STARTS[] = {"0.000", "0.001", "0.010", "0.020"};
const QString DEFAULT_MESOSCOPIC_DENSITY_RANGE_ENDS[] = {"0.001", "0.010", "0.020", "1.000"};
const QString DEFAULT_MESOSCOPIC_DENSITY_RANGE_COLORS[] = {"blue", "green", "yellow", "red"};

const QString MESOSCOPIC_FLOW_SETTING = "mesoscopic/flow";
const int DEFAULT_MESOSCOPIC_FLOW_RANGE_COUNT = 1;
const QString DEFAULT_MESOSCOPIC_FLOW_RANGE_STARTS[] = {"0.000"};
const QString DEFAULT_MESOSCOPIC_FLOW_RANGE_ENDS[] = {"1000.000"};
const QString DEFAULT_MESOSCOPIC_FLOW_RANGE_COLORS[] = {"blue"};

const QString MESOSCOPIC_SPEED_SETTING = "mesoscopic/speed";
const int DEFAULT_MESOSCOPIC_SPEED_RANGE_COUNT = 4;
const QString DEFAULT_MESOSCOPIC_SPEED_RANGE_STARTS[] = {"0.000", "500.000", "1000.000", "1500.000"};
const QString DEFAULT_MESOSCOPIC_SPEED_RANGE_ENDS[] = {"500.000", "1000.000", "1500.000", "2000.000"};
const QString DEFAULT_MESOSCOPIC_SPEED_RANGE_COLORS[] = {"red", "yellow", "green", "blue"};
}

PreferenceManager::PreferenceManager(QObject *parent) :
    QObject(parent) {
    settings_ = new QSettings("SMART", "iSimGUI");

    initDisplayed();
    initColor();
    initThreshold();
    initExtraInfo();
    initAgentIcon();
    initDBIConf();
    initMesoscopicColorRanges();
}

PreferenceManager::~PreferenceManager() {
    if (settings_) delete settings_;

    // remove mesoscopic range
    for (QList<RangeData*>::iterator i = mesoscopicDensityColorRangeList_.begin(); i != mesoscopicDensityColorRangeList_.end(); ++i) {
        delete *i;
    }
    for (QList<RangeData*>::iterator i = mesoscopicFlowColorRangeList_.begin(); i != mesoscopicFlowColorRangeList_.end(); ++i) {
        delete *i;
    }
    for (QList<RangeData*>::iterator i = mesoscopicSpeedColorRangeList_.begin(); i != mesoscopicSpeedColorRangeList_.end(); ++i) {
        delete *i;
    }
}

void PreferenceManager::initDisplayed() {
    isUniNodeShown_ = settings_->value(iSimGUI::UNINODE_DISPLAY_SETTING, iSimGUI::DEFAULT_UNINODES_IS_DISPLAYED).toBool();
    isMultiNodeShown_ = settings_->value(iSimGUI::MULTINODE_DISPLAY_SETTING, iSimGUI::DEFAULT_MULTINODES_IS_DISPLAYED).toBool();
    isSegmentShown_ = settings_->value(iSimGUI::SEGMENT_DISPLAY_SETTING, iSimGUI::DEFAULT_SEGMENTS_IS_DISPLAYED).toBool();
    isLaneShown_ = settings_->value(iSimGUI::LANE_DISPLAY_SETTING, iSimGUI::DEFAULT_LANES_IS_DISPLAYED).toBool();
    isLaneConnectorShown_ = settings_->value(iSimGUI::LANE_CONNECTORS_DISPLAY_SETTING, iSimGUI::DEFAULT_LANE_CONNECTORS_IS_DISPLAYED).toBool();
    isBusStopShown_ = settings_->value(iSimGUI::BUSSTOP_DISPLAY_SETTING, iSimGUI::DEFAULT_BUS_STOP_IS_DISPLAYED).toBool();
    isMicroscopicShown_ = settings_->value(iSimGUI::MICROSCOPIC_DISPLAY_SETTING, iSimGUI::DEFAULT_MICROSCOPIC_IS_DISPLAYED).toBool();
    isMesoscopicShown_ = settings_->value(iSimGUI::MESOSCOPIC_DISPLAY_SETTING, iSimGUI::DEFAULT_MESOSCOPIC_IS_DISPLAYED).toBool();
}

void PreferenceManager::initColor() {
    bgColor_ = QColor(settings_->value(iSimGUI::MAP_COLOR_SETTING, iSimGUI::DEFAULT_MAP_BG_COLOR).toString());
    uninodeColor_ = QColor(settings_->value(iSimGUI::UNINODE_COLOR_SETTING, iSimGUI::DEFAULT_UNINODE_COLOR).toString());
    multinodeColor_ = QColor(settings_->value(iSimGUI::MULTINODE_COLOR_SETTING, iSimGUI::DEFAULT_MULTINODE_COLOR).toString());
    segmentColor_ = QColor(settings_->value(iSimGUI::SEGMENT_COLOR_SETTING, iSimGUI::DEFAULT_SEGMENT_COLOR).toString());
    laneColor_ = QColor(settings_->value(iSimGUI::LANE_COLOR_SETTING, iSimGUI::DEFAULT_LANE_COLOR).toString());
    laneConnectorColor_ = QColor(settings_->value(iSimGUI::LANE_CONNECTOR_COLOR_SETTING, iSimGUI::DEFAULT_LANE_CONNECTOR_COLOR).toString());
}

void PreferenceManager::initThreshold() {
    uninodeThreshold_ = settings_->value(iSimGUI::UNINODE_THRESHOLD_SETTING, iSimGUI::DEFAULT_UNINODE_THRESHOLD).toInt();
    multinodeThreshold_ = settings_->value(iSimGUI::MULTINODE_THRESHOLD_SETTING, iSimGUI::DEFAULT_MULTINODE_THRESHOLD).toInt();
    segmentThreshold_ = settings_->value(iSimGUI::SEGMENT_THRESHOLD_SETTING, iSimGUI::DEFAULT_SEGMENT_THRESHOLD).toInt();
    laneThreshold_ = settings_->value(iSimGUI::LANE_THRESHOLD_SETTING, iSimGUI::DEFAULT_LANE_THRESHOLD).toInt();
    laneConnectorThreshold_ = settings_->value(iSimGUI::LANE_CONNECTOR_THRESHOLD_SETTING, iSimGUI::DEFAULT_LANE_CONNECTOR_THRESHOLD).toInt();
    busstopThreshold_ = settings_->value(iSimGUI::BUSSTOP_THRESHOLD_SETTING, iSimGUI::DEFAULT_BUSSTOP_THRESHOLD).toInt();
    microscopicThreshold_ = settings_->value(iSimGUI::MICROSCOPIC_THRESHOLD_SETTING, iSimGUI::DEFAULT_MICROSCOPIC_THRESHOLD).toInt();
}

void PreferenceManager::initExtraInfo() {
    uninodeExtraInfo_ = settings_->value(iSimGUI::UNINODE_EXTRA_INFO_SETTING, iSimGUI::DEFAULT_UNINODE_EXTRA_INFO).toInt();
    multinodeExtraInfo_ = settings_->value(iSimGUI::MULTINODE_EXTRA_INFO_SETTING, iSimGUI::DEFAULT_MULTINODE_EXTRA_INFO).toInt();
    segmentExtraInfo_ = settings_->value(iSimGUI::SEGMENT_EXTRA_INFO_SETTING, iSimGUI::DEFAULT_SEGMENT_EXTRA_INFO).toInt();
    laneExtraInfo_ = settings_->value(iSimGUI::LANE_EXTRA_INFO_SETTING, iSimGUI::DEFAULT_LANE_EXTRA_INFO).toInt();
}

void PreferenceManager::initAgentIcon() {
    driverIcon_ = settings_->value(iSimGUI::DRIVER_ICON_SETTING, 0).toInt();
    busIcon_ = settings_->value(iSimGUI::BUS_ICON_SETTING, 0).toInt();
    pedestrianIcon_ = settings_->value(iSimGUI::PEDESTRIAN_ICON_SETTING, 0).toInt();
}

void PreferenceManager::initDBIConf() {
    dbHost_ = settings_->value(iSimGUI::DB_HOST_SETTING, iSimGUI::DEFAULT_DB_HOST).toString();
    dbPort_ = settings_->value(iSimGUI::DB_PORT_SETTING, iSimGUI::DEFAULT_DB_PORT).toInt();
    dbUser_ = settings_->value(iSimGUI::DB_USERNAME_SETTING, iSimGUI::DEFAULT_DB_USERNAME).toString();
    dbPass_ = settings_->value(iSimGUI::DB_PASSWORD_SETTING, iSimGUI::DEFAULT_DB_PASSWORD).toString();
    dbName_ = settings_->value(iSimGUI::DB_NAME_SETTING, iSimGUI::DEFAULT_DB_NAME).toString();
}

void PreferenceManager::initMesoscopicColorRanges() {

    mesoscopicMode_ = settings_->value(iSimGUI::MESOSCOPIC_MODE_SETTING, iSimGUI::DEFAULT_MESOSCOPIC_MODE).toInt();

    // build density color range
    int densityRangeCount = settings_->value(iSimGUI::MESOSCOPIC_DENSITY_SETTING + "/count",
                                            iSimGUI::DEFAULT_MESOSCOPIC_DENSITY_RANGE_COUNT).toInt();
    for (int i = 0; i < densityRangeCount; i++) {
        double start = settings_->value(QString("%1/start-%2").arg(iSimGUI::MESOSCOPIC_DENSITY_SETTING).arg(QString::number(i)),
                                       iSimGUI::DEFAULT_MESOSCOPIC_DENSITY_RANGE_STARTS[i]).toDouble();
        double end = settings_->value(QString("%1/end-%2").arg(iSimGUI::MESOSCOPIC_DENSITY_SETTING).arg(QString::number(i)),
                                     iSimGUI::DEFAULT_MESOSCOPIC_DENSITY_RANGE_ENDS[i]).toDouble();
        QColor color(settings_->value(QString("%1/color-%2").arg(iSimGUI::MESOSCOPIC_DENSITY_SETTING).arg(QString::number(i)),
                                      iSimGUI::DEFAULT_MESOSCOPIC_DENSITY_RANGE_COLORS[i]).toString());
        color.setAlpha(settings_->value(QString("%1/alpha-%2").arg(iSimGUI::MESOSCOPIC_DENSITY_SETTING).arg(QString::number(i)),
                                        iSimGUI::DEFAULT_MESOSCOPIC_COLOR_ALPHA).toInt());
        mesoscopicDensityColorRangeList_.append(new RangeData(start, end, color));
    }

    // build speed color range
    int speedRangeCount = settings_->value(iSimGUI::MESOSCOPIC_SPEED_SETTING + "/count",
                                            iSimGUI::DEFAULT_MESOSCOPIC_SPEED_RANGE_COUNT).toInt();
    for (int i = 0; i < speedRangeCount; i++) {
        double start = settings_->value(QString("%1/start-%2").arg(iSimGUI::MESOSCOPIC_SPEED_SETTING).arg(QString::number(i)),
                                       iSimGUI::DEFAULT_MESOSCOPIC_SPEED_RANGE_STARTS[i]).toDouble();
        double end = settings_->value(QString("%1/end-%2").arg(iSimGUI::MESOSCOPIC_SPEED_SETTING).arg(QString::number(i)),
                                     iSimGUI::DEFAULT_MESOSCOPIC_SPEED_RANGE_ENDS[i]).toDouble();
        QColor color(settings_->value(QString("%1/color-%2").arg(iSimGUI::MESOSCOPIC_SPEED_SETTING).arg(QString::number(i)),
                                      iSimGUI::DEFAULT_MESOSCOPIC_SPEED_RANGE_COLORS[i]).toString());
        color.setAlpha(settings_->value(QString("%1/alpha-%2").arg(iSimGUI::MESOSCOPIC_SPEED_SETTING).arg(QString::number(i)),
                                        iSimGUI::DEFAULT_MESOSCOPIC_COLOR_ALPHA).toInt());
        mesoscopicSpeedColorRangeList_.append(new RangeData(start, end, color));
    }

    // build flow color range
    int flowRangeCount = settings_->value(iSimGUI::MESOSCOPIC_FLOW_SETTING + "/count",
                                            iSimGUI::DEFAULT_MESOSCOPIC_FLOW_RANGE_COUNT).toInt();
    for (int i = 0; i < flowRangeCount; i++) {
        double start = settings_->value(QString("%1/start-%2").arg(iSimGUI::MESOSCOPIC_FLOW_SETTING).arg(QString::number(i)),
                                       iSimGUI::DEFAULT_MESOSCOPIC_FLOW_RANGE_STARTS[i]).toDouble();
        double end = settings_->value(QString("%1/end-%2").arg(iSimGUI::MESOSCOPIC_FLOW_SETTING).arg(QString::number(i)),
                                     iSimGUI::DEFAULT_MESOSCOPIC_FLOW_RANGE_ENDS[i]).toDouble();
        QColor color(settings_->value(QString("%1/color-%2").arg(iSimGUI::MESOSCOPIC_FLOW_SETTING).arg(QString::number(i)),
                                      iSimGUI::DEFAULT_MESOSCOPIC_FLOW_RANGE_COLORS[i]).toString());
        color.setAlpha(settings_->value(QString("%1/alpha-%2").arg(iSimGUI::MESOSCOPIC_FLOW_SETTING).arg(QString::number(i)),
                                        iSimGUI::DEFAULT_MESOSCOPIC_COLOR_ALPHA).toInt());
        mesoscopicFlowColorRangeList_.append(new RangeData(start, end, color));
    }
}

void PreferenceManager::setMesoscopicMode(int mode) {
    if (mode == mesoscopicMode_) return;
    mesoscopicMode_ = mode;
    settings_->setValue(iSimGUI::MESOSCOPIC_MODE_SETTING, mode);
    emit updateMesosOverlay();
}

void PreferenceManager::setDBInfo(QString host, int port, QString username, QString password, QString dbName) {
    dbHost_ = host;
    settings_->setValue(iSimGUI::DB_HOST_SETTING, host);
    dbPort_ = port;
    settings_->setValue(iSimGUI::DB_PORT_SETTING, port);
    dbUser_ = username;
    settings_->setValue(iSimGUI::DB_USERNAME_SETTING, username);
    dbPass_ = password;
    settings_->setValue(iSimGUI::DB_PASSWORD_SETTING, password);
    dbName_ = dbName;
    settings_->setValue(iSimGUI::DB_NAME_SETTING, dbName);
    emit updateDBConf();
}

void PreferenceManager::setDriverIcon(int val) {
    driverIcon_ = val;
    settings_->setValue(iSimGUI::DRIVER_ICON_SETTING, val);
    emit updateAgents();
}

void PreferenceManager::setBusIcon(int val) {
    busIcon_ = val;
    settings_->setValue(iSimGUI::BUS_ICON_SETTING, val);
    emit updateAgents();
}

void PreferenceManager::setPedestrianIcon(int val) {
    pedestrianIcon_ = val;
    settings_->setValue(iSimGUI::PEDESTRIAN_ICON_SETTING, val);
    emit updateAgents();
}

void PreferenceManager::updateShownAttributes(iSimGUI::PreferenceType  type, bool value) {
    switch(type) {
        case iSimGUI::PREF_UNINODE:
                isUniNodeShown_ = value;
                settings_->setValue(iSimGUI::UNINODE_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MULTINODE:
                isMultiNodeShown_ = value;
                settings_->setValue(iSimGUI::MULTINODE_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_SEGMENT:
                isSegmentShown_ = value;
                settings_->setValue(iSimGUI::SEGMENT_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_LANE:
                isLaneShown_ = value;
                settings_->setValue(iSimGUI::LANE_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_LANE_CONNECTOR:
                isLaneConnectorShown_ = value;
                settings_->setValue(iSimGUI::LANE_CONNECTORS_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_BUSSTOP:
                isBusStopShown_ = value;
                settings_->setValue(iSimGUI::BUSSTOP_DISPLAY_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MICRO:
                isMicroscopicShown_ = value;
                settings_->setValue(iSimGUI::MICROSCOPIC_DISPLAY_SETTING, value);
                emit updateMicroData();
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MESOS:
                isMesoscopicShown_ = value;
                settings_->setValue(iSimGUI::MICROSCOPIC_DISPLAY_SETTING, value);
                emit updateMesosData();
                emit updateMapViewAttr();
                break;
    }
}

QColor PreferenceManager::getInvertBgColor() {
    return QColor(255 - bgColor_.red(), 255 - bgColor_.green(), 255 - bgColor_.blue());
}

void PreferenceManager::setBgColor(QColor color) {
    if (color != bgColor_) {
        bgColor_ = color;
        settings_->setValue(iSimGUI::MAP_COLOR_SETTING, color);
        emit updateBgColor();
    }
}

void PreferenceManager::updateColorAttributes(iSimGUI::PreferenceType  type, QColor color) {
    switch (type) {
        case iSimGUI::PREF_UNINODE:
                if (uninodeColor_ != color) {
                    uninodeColor_ = color;
                    settings_->setValue(iSimGUI::UNINODE_COLOR_SETTING, color);
                };
                break;
        case iSimGUI::PREF_MULTINODE:
                if (multinodeColor_ != color) {
                    multinodeColor_ = color;
                    settings_->setValue(iSimGUI::MULTINODE_COLOR_SETTING, color);
                };
                break;
        case iSimGUI::PREF_SEGMENT:
                if (segmentColor_ != color) {
                    segmentColor_ = color;
                    settings_->setValue(iSimGUI::SEGMENT_COLOR_SETTING, color);
                };
                break;
        case iSimGUI::PREF_LANE:
                if (laneColor_ != color) {
                    laneColor_ = color;
                    settings_->setValue(iSimGUI::LANE_COLOR_SETTING, color);
                };
                break;
        case iSimGUI::PREF_LANE_CONNECTOR:
                if (laneConnectorColor_ != color) {
                    laneConnectorColor_ = color;
                    settings_->setValue(iSimGUI::LANE_CONNECTOR_COLOR_SETTING, color);
                };
                break;
        default: break;
    }
    emit updateMapViewAttr();
}

void PreferenceManager::updateThresholdAttributes(iSimGUI::PreferenceType type, int value) {
    switch(type) {
        case iSimGUI::PREF_UNINODE:
                uninodeThreshold_ = value;
                settings_->setValue(iSimGUI::UNINODE_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MULTINODE:
                multinodeThreshold_ = value;
                settings_->setValue(iSimGUI::MULTINODE_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_SEGMENT:
                segmentThreshold_ = value;
                settings_->setValue(iSimGUI::SEGMENT_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_LANE:
                laneThreshold_ = value;
                settings_->setValue(iSimGUI::LANE_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_LANE_CONNECTOR:
                laneConnectorThreshold_ = value;
                settings_->setValue(iSimGUI::LANE_CONNECTOR_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_BUSSTOP:
                busstopThreshold_ = value;
                settings_->setValue(iSimGUI::BUSSTOP_THRESHOLD_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MICRO:
                microscopicThreshold_ = value;
                settings_->setValue(iSimGUI::MICROSCOPIC_THRESHOLD_SETTING, value);
                emit updateMicroData();
                break;
        default: break;
    }
}


void PreferenceManager::updateExtraInfoAttributes(iSimGUI::PreferenceType type, int value) {
    switch(type) {
        case iSimGUI::PREF_UNINODE:
                uninodeExtraInfo_ = value;
                settings_->setValue(iSimGUI::UNINODE_EXTRA_INFO_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_MULTINODE:
                multinodeExtraInfo_ = value;
                settings_->setValue(iSimGUI::MULTINODE_EXTRA_INFO_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_SEGMENT:
                segmentExtraInfo_ = value;
                settings_->setValue(iSimGUI::SEGMENT_EXTRA_INFO_SETTING, value);
                emit updateMapViewAttr();
                break;
        case iSimGUI::PREF_LANE:
                laneExtraInfo_ = value;
                settings_->setValue(iSimGUI::LANE_EXTRA_INFO_SETTING, value);
                emit updateMapViewAttr();
                break;
        default:
                break;
    }
}

void PreferenceManager::updateMesosColorRange(int mode, QColor color, int index) {
    QList<RangeData*> list;
    QString settingsPrefix;
    switch (mode) {
        case 0: list = mesoscopicDensityColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_DENSITY_SETTING;
                break;
        case 1: list = mesoscopicFlowColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_FLOW_SETTING;
                break;
        case 2: list = mesoscopicSpeedColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_SPEED_SETTING;
                break;
    }

    settings_->setValue(QString("%1/color-%2").arg(settingsPrefix).arg(QString::number(index)), color);
    settings_->setValue(QString("%1/alpha-%2").arg(settingsPrefix).arg(QString::number(index)), color.alpha());
    RangeData* range = list.at(index);
    range->setColor(color);

    emit updateMesosOverlay();
}

void PreferenceManager::updateMesosLowValue(int mode, double value, int index) {
    QList<RangeData*> list;
    QString settingsPrefix;
    switch (mode) {
        case 0: list = mesoscopicDensityColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_DENSITY_SETTING;
                break;
        case 1: list = mesoscopicFlowColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_FLOW_SETTING;
                break;
        case 2: list = mesoscopicSpeedColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_SPEED_SETTING;
                break;
    }

    settings_->setValue(QString("%1/start-%2").arg(settingsPrefix).arg(QString::number(index)), value);
    RangeData* range = list.at(index);
    range->setLow(value);

    emit updateMesosOverlay();
}

void PreferenceManager::updateMesosHighValue(int mode, double value, int index) {
    QList<RangeData*> list;
    QString settingsPrefix;
    switch (mode) {
        case 0: list = mesoscopicDensityColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_DENSITY_SETTING;
                break;
        case 1: list = mesoscopicFlowColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_FLOW_SETTING;
                break;
        case 2: list = mesoscopicSpeedColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_SPEED_SETTING;
                break;
    }

    settings_->setValue(QString("%1/end-%2").arg(settingsPrefix).arg(QString::number(index)), value);
    RangeData* range = list.at(index);
    range->setHigh(value);

    emit updateMesosOverlay();
}

RangeData* PreferenceManager::addNewMesosColorRange(int mode) {
    RangeData* range = new RangeData();
    QString settingsPrefix;
    int newSize = 0;
    switch (mode) {
        case 0: mesoscopicDensityColorRangeList_.append(range);
                newSize = mesoscopicDensityColorRangeList_.size();
                settingsPrefix = iSimGUI::MESOSCOPIC_DENSITY_SETTING;
                break;
        case 1: mesoscopicFlowColorRangeList_.append(range);
                newSize = mesoscopicFlowColorRangeList_.size();
                settingsPrefix = iSimGUI::MESOSCOPIC_FLOW_SETTING;
                break;
        case 2: mesoscopicSpeedColorRangeList_.append(range);
                newSize = mesoscopicSpeedColorRangeList_.size();
                settingsPrefix = iSimGUI::MESOSCOPIC_SPEED_SETTING;
                break;
    }
    settings_->setValue(QString("%1/color-%2").arg(settingsPrefix).arg(QString::number(newSize - 1)), range->getColor());
    settings_->setValue(settingsPrefix + "/count", newSize);

    emit updateMesosOverlay();
    return range;
}

void PreferenceManager::removeMesosColorRange(int mode, int index) {
    QList<RangeData*>* list = 0;
    QString settingsPrefix;
    switch (mode) {
        case 0: list = &mesoscopicDensityColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_DENSITY_SETTING;
                break;
        case 1: list = &mesoscopicFlowColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_FLOW_SETTING;
                break;
        case 2: list = &mesoscopicSpeedColorRangeList_;
                settingsPrefix = iSimGUI::MESOSCOPIC_SPEED_SETTING;
                break;
    }

    RangeData *range = list->at(index);
    list->removeAt(index);
    delete range;
    int newSize = list->size();
    settings_->setValue(settingsPrefix + "/count", newSize);
    for (int i = index; i < newSize; i++) {
        range = list->at(i);
        QColor color = range->getColor();
        settings_->setValue(QString("%1/color-%2").arg(settingsPrefix).arg(QString::number(i)), color);
        settings_->setValue(QString("%1/alpha-%2").arg(settingsPrefix).arg(QString::number(i)), color.alpha());
        settings_->setValue(QString("%1/start-%2").arg(settingsPrefix).arg(QString::number(i)), range->getLow());
        settings_->setValue(QString("%1/end-%2").arg(settingsPrefix).arg(QString::number(i)), range->getHigh());
    }

    emit updateMesosOverlay();
}

QColor& PreferenceManager::getMesosColorByRange(double value) {
    QList<RangeData*>* list;
    switch(mesoscopicMode_) {
        case 0 : list = &mesoscopicDensityColorRangeList_;
                 break;
        case 1 : list = &mesoscopicFlowColorRangeList_;
                 break;
        case 2 : list = &mesoscopicSpeedColorRangeList_;
                 break;
    }

    for (QList<RangeData*>::iterator i = list->begin(); i != list->end(); ++i) {
        RangeData *temp = *i;
        if (temp->isInRange(value))
            return temp->getColor();
    }

    return list->first()->getColor();
}

double PreferenceManager::getMesoscopicDensityPlotScale() {
    int lastRange = mesoscopicDensityColorRangeList_.size()-1;
    if (lastRange < 0) return 100;

    return mesoscopicDensityColorRangeList_[lastRange]->getLow()*10;
}

double PreferenceManager::getMesoscopicFlowPlotScale() {
    int lastRange = mesoscopicFlowColorRangeList_.size()-1;
    if (lastRange < 0) return 100;

    return mesoscopicFlowColorRangeList_[lastRange]->getLow()*10;
}

double PreferenceManager::getMesoscopicSpeedPlotScale() {
    int lastRange = mesoscopicSpeedColorRangeList_.size()-1;
    if (lastRange < 0) return 100;

    return mesoscopicSpeedColorRangeList_[lastRange]->getLow()*10;
}
