#ifndef DBIO_H
#define DBIO_H

#include <QString>
#include "commondef.h"


class DBio
{
public:
    DBio(QString dbName, iSimGUI::DataType type);
    ~DBio();
};

#endif // DBIO_H
