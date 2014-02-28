#include "g_agent.h"

G_Agent::G_Agent(QGraphicsItem *parent, PreferenceManager *preferenceManager)
    : QGraphicsObject(parent), preferenceManager_(preferenceManager) {

}

G_Agent::~G_Agent() {

}
