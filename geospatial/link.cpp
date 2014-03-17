#include "link.h"

const int Link::DB_ID = 1;

Link::Link(unsigned long id, QString name, unsigned long startNodeId, unsigned long endNodeId)
{
    name_ = name;
    id_ = id;
    startNodeId_ = startNodeId;
    endNodeId_ = endNodeId;
}

Link::Link(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    name_ = attributes[0];
    startNodeId_ = attributes[1].toULongLong();
    endNodeId_ = attributes[2].toULongLong();
    QStringList fwdSegmentIds = attributes[3].split(",");
    for (int i = 0; i < fwdSegmentIds.size(); i++)
        fwdSegmentIds_.append(fwdSegmentIds[i].toULongLong());
    QStringList revSegmentIds = attributes[4].split(",");
    for (int i = 0; i < revSegmentIds.size(); i++)
        revSegmentIds_.append(revSegmentIds[i].toULongLong());
}

QString Link::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", 1";

    QStringList info;
    info << name_;
    info << QString::number(startNodeId_);
    info << QString::number(endNodeId_);

    QStringList fwdSegmentIds;
    for (int i = 0; i < fwdSegmentIds_.size(); i++)
        fwdSegmentIds << QString::number(fwdSegmentIds_[i]);
    info << fwdSegmentIds.join(",");

    QStringList revSegmentIds;
    for (int i = 0; i < revSegmentIds_.size(); i++)
        revSegmentIds << QString::number(revSegmentIds_[i]);
    info << revSegmentIds.join(",");

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}
