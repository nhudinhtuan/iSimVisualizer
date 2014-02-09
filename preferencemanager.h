#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

#include <QObject>
#include <QSettings>

class PreferenceManager : public QObject
{
    Q_OBJECT
public:
    explicit PreferenceManager(QObject *parent = 0);
    ~PreferenceManager();

signals:

public slots:
    void buildIsDisplayed();

private:
    QSettings *settings_;

    bool isMultiNodeShown_;
    bool isUniNodeShown_;
    bool isRoadSegmentShown_;
    bool isLaneShown_;
    bool isBusStopShown_;
};

#endif // PREFERENCEMANAGER_H
