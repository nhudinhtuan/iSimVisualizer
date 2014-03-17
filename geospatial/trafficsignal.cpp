#include "trafficsignal.h"

const int TrafficSignal::DB_ID = 6;

TrafficSignal::TrafficSignal(unsigned long id, unsigned long nodeId)
{
    id_ = id;
    nodeId_ = nodeId;
}

TrafficSignal::TrafficSignal(unsigned long id, QString& dbInfo) {
    id_ = id;
    QStringList attributes = dbInfo.split("||");
    nodeId_ = attributes[0].toULongLong();
    QStringList phaseStr = attributes[1].split(",");
    for (int i = 0; i < phaseStr.size(); i++) {
        TrafficPhase *phase = new TrafficPhase(phaseStr[i], true);
        phases_[phase->name] = phase;
    }
}

TrafficSignal::~TrafficSignal() {
    QHash<QString, TrafficPhase*>::iterator phaseIt = phases_.begin();
    while (phaseIt != phases_.end()) {
        delete phaseIt.value();
        phaseIt++;
    }
}

void TrafficSignal::addPhase(QString name, TrafficPhase* phase) {
    phases_[name] = phase;
}

TrafficPhase* TrafficSignal::getPhase(QString name) {
    if (phases_.contains(name)) return 0;
    return phases_[name];
}

QString TrafficSignal::sqlInsertValue() const {
    QString result = "(";
    result += QString::number(id_);
    result += ", " + QString::number(DB_ID);

    QStringList info;
    info << QString::number(nodeId_);

    QStringList phaseStr;
    QHash<QString, TrafficPhase*>::const_iterator phaseIt = phases_.begin();
    while (phaseIt != phases_.end()) {
        phaseStr << phaseIt.value()->serialize();
        phaseIt++;
    }
    info << phaseStr.join(",");

    result += ", \'" + info.join("||") + "\'";
    result += ")";
    return result;
}
