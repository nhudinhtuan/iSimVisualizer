#ifndef G_AGENTFACTORY_H
#define G_AGENTFACTORY_H

#include "preferencemanager.h"
#include "temporal/agent.h"
#include "temporal/g_agent.h"
#include "temporal/g_driver.h"
#include "temporal/g_busdriver.h"
#include "temporal/g_pedestrian.h"

class G_AgentFactory
{
public:
    static G_Agent* create(Agent*, PreferenceManager*);
};

#endif // G_AGENTFACTORY_H
