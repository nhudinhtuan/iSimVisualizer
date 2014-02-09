#ifndef COMMONDEF_H
#define COMMONDEF_H
#include <QString>

namespace iSimGUI{

    enum DataType {DATA_SHORT_TERM = 0, DATA_MEDIUM_TERM = 1, DATA_SIM_XML = 2};
    enum AgentType {AGENT_DRIVER = 0, AGENT_BUS = 1, AGENT_PEDESTRIAN = 2};
    const QString INI_FILE = "iSimGUI.ini";
    const long MAX_NUM_LINE_IN_CSV = 200000;//50000;
    const unsigned long ZOOM_LEVEL_ZERO_WIDTH = 7.2518e+06;
    const unsigned long ZOOM_LEVEL_ZERO_HEIGHT = 2.74347e+06;

}
#endif // COMMONDEF_H
