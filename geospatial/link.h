#ifndef LINK_H
#define LINK_H

#include <QList>
#include <QString>

class Link
{
public:
    Link(unsigned long id, QString name, unsigned long startNodeId, unsigned long endNodeId);
    QString& getName() {return name_;}
    unsigned long getId() {return id_;}
    unsigned long getStartNodeId() {return startNodeId_;}
    unsigned long getEndNodeId() {return endNodeId_;}
    QList<unsigned long>& getFwdSegmentIds() {return fwdSegmentIds_;}
    QList<unsigned long>& getRevSegmentIds() {return revSegmentIds_;}

    void addFwdSegmentId(unsigned long id) {fwdSegmentIds_.append(id);}
    void addRevSegmentId(unsigned long id) {revSegmentIds_.append(id);}

private:
    QString name_;
    unsigned long id_;
    unsigned long startNodeId_;
    unsigned long endNodeId_;
    QList<unsigned long> fwdSegmentIds_;
    QList<unsigned long> revSegmentIds_;
};

#endif // LINK_H
