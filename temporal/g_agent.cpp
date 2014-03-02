#include "g_agent.h"

G_Agent::G_Agent(QGraphicsItem *parent, PreferenceManager *preferenceManager, MapGraphicsView *mapView)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager), mapView_(mapView) {

}

G_Agent::~G_Agent() {

}
