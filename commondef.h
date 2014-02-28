#ifndef COMMONDEF_H
#define COMMONDEF_H
#include <QString>

namespace iSimGUI{

    enum DataType {DATA_SHORT_TERM = 0, DATA_MEDIUM_TERM = 1, DATA_SIM_XML = 2};
    const QString INI_FILE = "iSimGUI.ini";
    const long MAX_NUM_LINE_IN_CSV = 200000;//50000;
}
#endif // COMMONDEF_H
