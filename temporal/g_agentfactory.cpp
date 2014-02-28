#include "g_agentfactory.h"

G_Agent* G_AgentFactory::create(Agent* model, PreferenceManager* preferenceManager) {
    switch(model->getType()) {
        case iSimGUI::AGENT_DRIVER: {
            Driver* driver = dynamic_cast<Driver*>(model);
            return new G_Driver(0, driver, preferenceManager);
        }
        case iSimGUI::AGENT_BUS: {
            BusDriver* bus = dynamic_cast<BusDriver*>(model);
            return new G_BusDriver(0, bus, preferenceManager);
        }
        case iSimGUI::AGENT_PEDESTRIAN: {
            Pedestrian* pedestrian = dynamic_cast<Pedestrian*>(model);
            return new G_Pedestrian(0, pedestrian, preferenceManager);
        }
    }
    return 0;
}
