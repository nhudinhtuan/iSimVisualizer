#include "UtilityFunctions.h"

QString genDBNames(QString& fileName) {
    QFileInfo fileInfo(fileName);
    int fileSize = fileInfo.size()*100/(1024*1024);
    double preciseFileSize = fileSize;
    preciseFileSize /= 100;
    std::string fileBaseName = fileInfo.baseName().toStdString();
    fileBaseName = fileBaseName.substr(0, 17);
    std::ostringstream dbfileNameStream;
    dbfileNameStream << fileBaseName;
    dbfileNameStream << "_";
    dbfileNameStream << preciseFileSize;
    QString dbname;
    dbname = dbname.fromStdString(dbfileNameStream.str());
    dbname.replace('.', '_');
    return dbname;
}
