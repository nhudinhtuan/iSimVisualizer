#include "preferencemanager.h"

namespace iSimGUI {

const bool DEFAULT_MULTINODES_IS_DISPLAYED = true;
const bool DEFAULT_UNINODES_IS_DISPLAYED = true;
const bool DEFAULT_ROAD_SEGMENTS_IS_DISPLAYED = true;
const bool DEFAULT_LANES_IS_DISPLAYED = true;
const bool DEFAULT_AGENT_IS_DISPLAYED = true;
const bool DEFAULT_BUS_STOP_IS_DISPLAYED = true;

}

PreferenceManager::PreferenceManager(QObject *parent) :
    QObject(parent) {
    settings_ = new QSettings("SMART", "iSimGUI");

}

PreferenceManager::~PreferenceManager() {
    if (settings_) delete settings_;
}

void PreferenceManager::buildIsDisplayed() {
    isMultiNodeShown_ = settings_->value("graphics/multiNodes/isDisplayed", iSimGUI::DEFAULT_MULTINODES_IS_DISPLAYED).toBool();
    isUniNodeShown_ = settings_->value("graphics/uniNodes/isDisplayed", iSimGUI::DEFAULT_UNINODES_IS_DISPLAYED).toBool();
    isRoadSegmentShown_ = settings_->value("graphics/roadSegments/isDisplayed", iSimGUI::DEFAULT_ROAD_SEGMENTS_IS_DISPLAYED).toBool();
    isLaneShown_ = settings_->value("graphics/lanes/isDisplayed", iSimGUI::DEFAULT_LANES_IS_DISPLAYED).toBool();
    isBusStopShown_ = settings_->value("graphics/busStop/isDisplayed", iSimGUI::DEFAULT_BUS_STOP_IS_DISPLAYED).toBool();
}
