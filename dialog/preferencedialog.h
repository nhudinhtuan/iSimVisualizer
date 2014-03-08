#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QObject>
#include <QSettings>
#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialog>
#include <QColorDialog>
#include "preferencemanager.h"
#include "io/dbhandler.h"

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog {
    Q_OBJECT

public:
    PreferenceDialog(QWidget* parent, PreferenceManager* preferenceManager);
    virtual ~PreferenceDialog();

public slots:
    void triggerUninodeCheckbox(bool);
    void triggerMultinodeCheckbox(bool);
    void triggerSegmentCheckbox(bool);
    void triggerLaneCheckbox(bool);
    void triggerLaneConnectorCheckbox(bool);
    void triggerBusStopCheckbox(bool);
    void triggerMicroscopicCheckbox(bool);
    void triggerMesoscopicCheckbox(bool);

    void selectBgColor();
    void selectUninodeColor();
    void selectMultinodeColor();
    void selectSegmentColor();
    void selectLaneColor();
    void selectLaneConnectorColor();

    void changeUninodeThreshold(int);
    void changeMultinodeThreshold(int);
    void changeSegmentThreshold(int);
    void changeLaneThreshold(int);
    void changeLaneConnectorThreshold(int);
    void changeBusstopThreshold(int);
    void changeMicroscopicThreshold(int);

    void changeUninodeExtraInfo(int);
    void changeMultinodeExtraInfo(int);
    void changeSegmentExtraInfo(int);
    void changeLaneExtraInfo(int);

    void changeDriverIcon(int);
    void changeBusIcon(int);
    void changePedestrianIcon(int);

    void testDBConnection();
    void resetDBUI();
    void saveDBConf();

    void changeMesoscopicMode(int);
    void updateMesosDensityColor(int, int);
    void updateMesosFlowColor(int, int);
    void updateMesosSpeedColor(int, int);
    void addNewMesosDensityRange();
    void addNewMesosFlowRange();
    void addNewMesosSpeedRange();
    void editDensityRange(QTableWidgetItem *item);
    void editFlowRange(QTableWidgetItem *item);
    void editSpeedRange(QTableWidgetItem *item);
    void removeSelectedDensityRange();
    void removeSelectedFlowRange();
    void removeSelectedSpeedRange();

private:

    void initUI();
    void initSignal();
    void setButtonColor(QPushButton* button, QColor color);

    Ui::PreferenceDialog *ui_;
    PreferenceManager* preferenceManager_;
};

#endif // PREFERENCEDIALOG_H
