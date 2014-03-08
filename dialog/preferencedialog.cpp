#include "preferencedialog.h"
#include "ui_preferencedialog.h"

PreferenceDialog::PreferenceDialog(QWidget* parent, PreferenceManager* preferenceManager) :
    QDialog(parent), ui_(new Ui::PreferenceDialog), preferenceManager_(preferenceManager) {
    ui_->setupUi(this);
    setWindowTitle(tr("Preferences"));
    setModal(true);

    initUI();
    initSignal();
}

PreferenceDialog::~PreferenceDialog() {
    if (ui_) delete ui_;
}

void PreferenceDialog::initUI() {
    setButtonColor(ui_->bg_color, preferenceManager_->getBgColor());

    ui_->uninodeCheckBox->setChecked(preferenceManager_->isUninodeDisplayed());
    ui_->multinodeCheckBox->setChecked(preferenceManager_->isMultinodeDisplayed());
    ui_->segmentCheckBox->setChecked(preferenceManager_->isSegmentDisplayed());
    ui_->laneCheckBox->setChecked(preferenceManager_->isLaneDisplayed());
    ui_->laneConCheckBox->setChecked(preferenceManager_->isLaneConnectorDisplayed());
    ui_->busstopCheckBox->setChecked(preferenceManager_->isBusStopDisplayed());
    ui_->microCheckbox->setChecked(preferenceManager_->isMicroscopicDisplayed());
    ui_->mesosCheckbox->setChecked(preferenceManager_->isMesoscopicDisplayed());

    setButtonColor(ui_->uninodeColor, preferenceManager_->getUninodeColor());
    setButtonColor(ui_->multinodeColor, preferenceManager_->getMultinodeColor());
    setButtonColor(ui_->segmentColor, preferenceManager_->getSegmentColor());
    setButtonColor(ui_->laneColor, preferenceManager_->getLaneColor());
    setButtonColor(ui_->laneConColor, preferenceManager_->getLaneConnectorColor());

    ui_->uninodeSpinBox->setValue(preferenceManager_->getUninodeThreshold());
    ui_->multinodeSpinBox->setValue(preferenceManager_->getMultinodeThreshold());
    ui_->segmentSpinBox->setValue(preferenceManager_->getSegmentThreshold());
    ui_->laneSpinBox->setValue(preferenceManager_->getLaneThreshold());
    ui_->laneConSpinBox->setValue(preferenceManager_->getLaneConnectorThreshold());
    ui_->busstopSpinBox->setValue(preferenceManager_->getBusstopThreshold());
    ui_->microSpinBox->setValue(preferenceManager_->getMicroscopicThreshold());

    //db
    ui_->hostInput->setText(preferenceManager_->getDBHost());
    ui_->portInput->setValue(preferenceManager_->getDBPort());
    ui_->usernameInput->setText(preferenceManager_->getDBUsername());
    ui_->passwordInput->setText(preferenceManager_->getDBPass());
    ui_->dbNameInput->setText(preferenceManager_->getDBName());

    // geospatial extra info
    for (int i = 0; i < iSimGUI::N_NODE_EXTRA_INFO; i++) {
        ui_->uninodeExtraInfo->addItem(iSimGUI::NODE_EXTRA_INFO[i]);
        ui_->multinodeExtraInfo->addItem(iSimGUI::NODE_EXTRA_INFO[i]);
    }
    ui_->uninodeExtraInfo->setCurrentIndex(preferenceManager_->getUninodeExtraInfo());
    ui_->multinodeExtraInfo->setCurrentIndex(preferenceManager_->getMultinodeExtraInfo());

    for (int i = 0; i < iSimGUI::N_SEGMENT_EXTRA_INFO; i++)
        ui_->segmentExtraInfo->addItem(iSimGUI::SEGMENT_EXTRA_INFO[i]);
    ui_->segmentExtraInfo->setCurrentIndex(preferenceManager_->getSegmentExtraInfo());

    for (int i = 0; i < iSimGUI::N_LANE_EXTRA_INFO; i++)
        ui_->laneExtraInfo->addItem(iSimGUI::LANE_EXTRA_INFO[i]);
    ui_->laneExtraInfo->setCurrentIndex(preferenceManager_->getLaneExtraInfo());

    //Agent Icon Qcombobox
    for (int i = 0; i < iSimGUI::N_DRIVER_ICON; i++)
        ui_->driverComboBox->addItem(iSimGUI::ICON_DRIVERS[i]);
    ui_->driverComboBox->setCurrentIndex(preferenceManager_->getDriverIconIndex());
    ui_->driverImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + preferenceManager_->getDriverIcon()));

    for (int i = 0; i < iSimGUI::N_BUS_ICON; i++)
        ui_->busComboBox->addItem(iSimGUI::ICON_BUSES[i]);
    ui_->busComboBox->setCurrentIndex(preferenceManager_->getBusIconIndex());
    ui_->busImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + preferenceManager_->getBusIcon()));

    for (int i = 0; i < iSimGUI::N_PEDESTRIAN_ICON; i++)
        ui_->pedestrianComboBox->addItem(iSimGUI::ICON_PEDESTRIANS[i]);
    ui_->pedestrianComboBox->setCurrentIndex(preferenceManager_->getPedestrianIconIndex());
    ui_->pedestrianImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + preferenceManager_->getPedestrianIcon()));


    // init mesoscopic
    int rowIndex = 0;
    QList<RangeData*>& mesDensityRanges = preferenceManager_->getMesoscopicDensityColorRanges();
    ui_->mesDensityTable->setRowCount(mesDensityRanges.size());
    for (QList<RangeData*>::iterator densityIt = mesDensityRanges.begin(); densityIt != mesDensityRanges.end(); ++densityIt) {
        RangeData* range = *densityIt;

        QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
        QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
        QTableWidgetItem *colorItem = new QTableWidgetItem();
        lowItem->setTextAlignment(Qt::AlignCenter);
        highItem->setTextAlignment(Qt::AlignCenter);
        colorItem->setFlags(Qt::ItemIsSelectable);
        colorItem->setBackgroundColor(range->getColor());
        ui_->mesDensityTable->setItem(rowIndex, 0, lowItem);
        ui_->mesDensityTable->setItem(rowIndex, 1, highItem);
        ui_->mesDensityTable->setItem(rowIndex, 2, colorItem);
        rowIndex++;
    }

    QList<RangeData*>& mesFlowRanges = preferenceManager_->getMesoscopicFlowColorRanges();
    ui_->mesFlowTable->setRowCount(mesFlowRanges.size());
    rowIndex = 0;
    for (QList<RangeData*>::iterator flowIt = mesFlowRanges.begin(); flowIt != mesFlowRanges.end(); ++flowIt) {
        RangeData* range = *flowIt;

        QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
        QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
        QTableWidgetItem *colorItem = new QTableWidgetItem();
        lowItem->setTextAlignment(Qt::AlignCenter);
        highItem->setTextAlignment(Qt::AlignCenter);
        colorItem->setFlags(Qt::ItemIsSelectable);
        colorItem->setBackgroundColor(range->getColor());
        ui_->mesFlowTable->setItem(rowIndex, 0, lowItem);
        ui_->mesFlowTable->setItem(rowIndex, 1, highItem);
        ui_->mesFlowTable->setItem(rowIndex, 2, colorItem);
        rowIndex++;
    }

    QList<RangeData*>& mesSpeedRanges = preferenceManager_->getMesoscopicSpeedColorRanges();
    ui_->mesSpeedTable->setRowCount(mesSpeedRanges.size());
    rowIndex = 0;
    for (QList<RangeData*>::iterator speedIt = mesSpeedRanges.begin(); speedIt != mesSpeedRanges.end(); ++speedIt) {
        RangeData* range = *speedIt;

        QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
        QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
        QTableWidgetItem *colorItem = new QTableWidgetItem();
        lowItem->setTextAlignment(Qt::AlignCenter);
        highItem->setTextAlignment(Qt::AlignCenter);
        colorItem->setFlags(Qt::ItemIsSelectable);
        colorItem->setBackgroundColor(range->getColor());
        ui_->mesSpeedTable->setItem(rowIndex, 0, lowItem);
        ui_->mesSpeedTable->setItem(rowIndex, 1, highItem);
        ui_->mesSpeedTable->setItem(rowIndex, 2, colorItem);
        rowIndex++;
    }

    int mesMode = preferenceManager_->getMesoscopicMode();
    ui_->mesStackedWidget->setCurrentIndex(mesMode);
    ui_->mesMode->setCurrentIndex(mesMode);
}

void PreferenceDialog::setButtonColor(QPushButton* button, QColor color) {
    QPalette palette;
    palette.setColor(QPalette::Button, color);
    button->setPalette(palette);
}

void PreferenceDialog::triggerUninodeCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_UNINODE, value);
}

void PreferenceDialog::triggerMultinodeCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_MULTINODE, value);
}

void PreferenceDialog::triggerSegmentCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_SEGMENT, value);
}

void PreferenceDialog::triggerLaneCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_LANE, value);
}

void PreferenceDialog::triggerLaneConnectorCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_LANE_CONNECTOR, value);
}

void PreferenceDialog::triggerBusStopCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_BUSSTOP, value);
}

void PreferenceDialog::triggerMicroscopicCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_MICRO, value);
}

void PreferenceDialog::triggerMesoscopicCheckbox(bool value) {
    preferenceManager_->updateShownAttributes(iSimGUI::PREF_MESOS, value);
}

void PreferenceDialog::selectBgColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->bg_color->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->bg_color, newColor);
        preferenceManager_->setBgColor(newColor);
    }
}

void PreferenceDialog::selectUninodeColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->uninodeColor->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->uninodeColor, newColor);
        preferenceManager_->updateColorAttributes(iSimGUI::PREF_UNINODE, newColor);
    }
}

void PreferenceDialog::selectMultinodeColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->multinodeColor->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->multinodeColor, newColor);
        preferenceManager_->updateColorAttributes(iSimGUI::PREF_MULTINODE, newColor);
    }
}

void PreferenceDialog::selectSegmentColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->segmentColor->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->segmentColor, newColor);
        preferenceManager_->updateColorAttributes(iSimGUI::PREF_SEGMENT, newColor);
    }
}

void PreferenceDialog::selectLaneColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->laneColor->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->laneColor, newColor);
        preferenceManager_->updateColorAttributes(iSimGUI::PREF_LANE, newColor);
    }
}

void PreferenceDialog::selectLaneConnectorColor() {
    QColorDialog colorPicker;
    QColor newColor(colorPicker.getColor(ui_->laneConColor->palette().color(QPalette::Button), this, tr("Choose Color")));
    if (newColor.isValid()) {
        setButtonColor(ui_->laneConColor, newColor);
        preferenceManager_->updateColorAttributes(iSimGUI::PREF_LANE_CONNECTOR, newColor);
    }
}

void PreferenceDialog::changeUninodeThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_UNINODE, value);
}

void PreferenceDialog::changeMultinodeThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_MULTINODE, value);
}

void PreferenceDialog::changeSegmentThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_SEGMENT, value);
}

void PreferenceDialog::changeLaneThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_LANE, value);
}

void PreferenceDialog::changeLaneConnectorThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_LANE_CONNECTOR, value);
}

void PreferenceDialog::changeBusstopThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_BUSSTOP, value);
}

void PreferenceDialog::changeMicroscopicThreshold(int value) {
    preferenceManager_->updateThresholdAttributes(iSimGUI::PREF_MICRO, value);
}

void PreferenceDialog::changeUninodeExtraInfo(int value) {
    preferenceManager_->updateExtraInfoAttributes(iSimGUI::PREF_UNINODE, value);
}

void PreferenceDialog::changeMultinodeExtraInfo(int value) {
    preferenceManager_->updateExtraInfoAttributes(iSimGUI::PREF_MULTINODE, value);
}

void PreferenceDialog::changeSegmentExtraInfo(int value) {
    preferenceManager_->updateExtraInfoAttributes(iSimGUI::PREF_SEGMENT, value);
}

void PreferenceDialog::changeLaneExtraInfo(int value) {
    preferenceManager_->updateExtraInfoAttributes(iSimGUI::PREF_LANE, value);
}

void PreferenceDialog::changeDriverIcon(int value) {
    preferenceManager_->setDriverIcon(value);
    ui_->driverImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + iSimGUI::ICON_DRIVERS[value]));
}

void PreferenceDialog::changeBusIcon(int value) {
    preferenceManager_->setBusIcon(value);
    ui_->busImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + iSimGUI::ICON_BUSES[value]));
}

void PreferenceDialog::changePedestrianIcon(int value) {
    preferenceManager_->setPedestrianIcon(value);
    ui_->busImg->setPixmap(QPixmap(iSimGUI::ICON_DIRECTORY + iSimGUI::ICON_PEDESTRIANS[value]));
}

void PreferenceDialog::initSignal() {
    connect(ui_->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui_->bg_color, SIGNAL(clicked()), this, SLOT(selectBgColor()));

    // checkbox for geospatial
    connect(ui_->uninodeCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerUninodeCheckbox(bool)));
    connect(ui_->uninodeCheckBox, SIGNAL(toggled(bool)), ui_->uninodeSpinBox, SLOT(setEnabled(bool)));
    connect(ui_->multinodeCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerMultinodeCheckbox(bool)));
    connect(ui_->multinodeCheckBox, SIGNAL(toggled(bool)), ui_->multinodeSpinBox, SLOT(setEnabled(bool)));
    connect(ui_->segmentCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerSegmentCheckbox(bool)));
    connect(ui_->segmentCheckBox, SIGNAL(toggled(bool)), ui_->segmentSpinBox, SLOT(setEnabled(bool)));
    connect(ui_->laneCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerLaneCheckbox(bool)));
    connect(ui_->laneCheckBox, SIGNAL(toggled(bool)), ui_->laneSpinBox, SLOT(setEnabled(bool)));
    connect(ui_->laneConCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerLaneConnectorCheckbox(bool)));
    connect(ui_->laneConCheckBox, SIGNAL(toggled(bool)), ui_->laneConSpinBox, SLOT(setEnabled(bool)));
    connect(ui_->busstopCheckBox, SIGNAL(toggled(bool)), this, SLOT(triggerBusStopCheckbox(bool)));
    connect(ui_->busstopCheckBox, SIGNAL(toggled(bool)), ui_->busstopSpinBox, SLOT(setEnabled(bool)));

    // checkbox for microscopic
    connect(ui_->microCheckbox, SIGNAL(toggled(bool)), this, SLOT(triggerMicroscopicCheckbox(bool)));
    connect(ui_->microCheckbox, SIGNAL(toggled(bool)), ui_->microSpinBox, SLOT(setEnabled(bool)));

    // checkbox for mesoscopic
    connect(ui_->mesosCheckbox, SIGNAL(toggled(bool)), this, SLOT(triggerMesoscopicCheckbox(bool)));

    // color
    connect(ui_->uninodeColor, SIGNAL(clicked()), this, SLOT(selectUninodeColor()));
    connect(ui_->multinodeColor, SIGNAL(clicked()), this, SLOT(selectMultinodeColor()));
    connect(ui_->segmentColor, SIGNAL(clicked()), this, SLOT(selectSegmentColor()));
    connect(ui_->laneColor, SIGNAL(clicked()), this, SLOT(selectLaneColor()));
    connect(ui_->laneConColor, SIGNAL(clicked()), this, SLOT(selectLaneConnectorColor()));

    // threshold
    connect(ui_->uninodeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeUninodeThreshold(int)));
    connect(ui_->multinodeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeMultinodeThreshold(int)));
    connect(ui_->segmentSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeSegmentThreshold(int)));
    connect(ui_->laneSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeLaneThreshold(int)));
    connect(ui_->laneConSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeLaneConnectorThreshold(int)));
    connect(ui_->busstopSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeBusstopThreshold(int)));
    connect(ui_->microSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeMicroscopicThreshold(int)));

    connect(ui_->driverComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeDriverIcon(int)));
    connect(ui_->busComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBusIcon(int)));
    connect(ui_->pedestrianComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changePedestrianIcon(int)));

    connect(ui_->uninodeExtraInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeUninodeExtraInfo(int)));
    connect(ui_->multinodeExtraInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMultinodeExtraInfo(int)));
    connect(ui_->segmentExtraInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSegmentExtraInfo(int)));
    connect(ui_->laneExtraInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLaneExtraInfo(int)));

    // db
    connect(ui_->testConBut, SIGNAL(clicked()), this, SLOT(testDBConnection()));
    connect(ui_->hostInput, SIGNAL(textEdited(const QString &)), this, SLOT(resetDBUI()));
    connect(ui_->usernameInput, SIGNAL(textEdited(const QString &)), this, SLOT(resetDBUI()));
    connect(ui_->passwordInput, SIGNAL(textEdited(const QString &)), this, SLOT(resetDBUI()));
    connect(ui_->dbNameInput, SIGNAL(textEdited(const QString &)), this, SLOT(resetDBUI()));
    connect(ui_->portInput, SIGNAL(valueChanged(int)), this, SLOT(resetDBUI()));
    connect(ui_->saveDbBut, SIGNAL(clicked()), this, SLOT(saveDBConf()));

    // mesoscopic
    connect(ui_->mesMode, SIGNAL(activated(int)), this, SLOT(changeMesoscopicMode(int)));
    connect(ui_->mesDensityTable, SIGNAL(cellClicked(int, int)), this, SLOT(updateMesosDensityColor(int, int)));
    connect(ui_->mesFlowTable, SIGNAL(cellClicked(int, int)), this, SLOT(updateMesosFlowColor(int, int)));
    connect(ui_->mesSpeedTable, SIGNAL(cellClicked(int, int)), this, SLOT(updateMesosSpeedColor(int, int)));
    connect(ui_->addDensityRangeBut, SIGNAL(clicked()), this, SLOT(addNewMesosDensityRange()));
    connect(ui_->addFlowRangeBut, SIGNAL(clicked()), this, SLOT(addNewMesosFlowRange()));
    connect(ui_->addSpeedRangeBut, SIGNAL(clicked()), this, SLOT(addNewMesosSpeedRange()));
    connect(ui_->removeDensityRangeBut, SIGNAL(clicked()), this, SLOT(removeSelectedDensityRange()));
    connect(ui_->removeFlowRangeBut, SIGNAL(clicked()), this, SLOT(removeSelectedFlowRange()));
    connect(ui_->removeSpeedRangeBut, SIGNAL(clicked()), this, SLOT(removeSelectedSpeedRange()));
    connect(ui_->mesDensityTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editDensityRange(QTableWidgetItem*)));
    connect(ui_->mesFlowTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editFlowRange(QTableWidgetItem*)));
    connect(ui_->mesSpeedTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(editSpeedRange(QTableWidgetItem*)));

}

void PreferenceDialog::testDBConnection() {
    QString host = ui_->hostInput->text();
    QString user = ui_->usernameInput->text();
    QString pass = ui_->passwordInput->text();
    QString dbName = ui_->dbNameInput->text();
    if (host.isEmpty() || user.isEmpty() || dbName.isEmpty()) {
        ui_->dbMessage->setText(tr("Error: Please enter all db information!"));
        return;
    }
    int port = ui_->portInput->value();
    QString error;
    if (DBHandler::testConnection(host, port, user, pass, dbName, &error)) {
        ui_->dbMessage->setText(tr("Connecting Successfully!"));
        ui_->saveDbBut->setDisabled(false);
        ui_->testConBut->setDisabled(true);
    } else {
        ui_->dbMessage->setText(error);
    }
}

void PreferenceDialog::resetDBUI() {
    ui_->dbMessage->setText("");
    ui_->testConBut->setDisabled(false);
    ui_->saveDbBut->setDisabled(true);
}

void PreferenceDialog::saveDBConf() {
    QString host = ui_->hostInput->text();
    QString user = ui_->usernameInput->text();
    QString pass = ui_->passwordInput->text();
    QString dbName = ui_->dbNameInput->text();
    int port = ui_->portInput->value();
    preferenceManager_->setDBInfo(host, port, user, pass, dbName);
    ui_->dbMessage->setText(tr("The new DB Configuration is saved successfully. The workspace is reset."));
    ui_->saveDbBut->setDisabled(true);
}

void PreferenceDialog::changeMesoscopicMode(int mode) {
    ui_->mesStackedWidget->setCurrentIndex(mode);
    preferenceManager_->setMesoscopicMode(mode);
}

void PreferenceDialog::updateMesosDensityColor(int row, int column) {
    // Only column 2 is color 'selectible'
    if (column != 2) return;
    QColorDialog colorPicker;
    QColor color(colorPicker.getColor(ui_->mesDensityTable->item(row, column)->backgroundColor(), this, tr("Choose Color"), QColorDialog::ShowAlphaChannel));
    if (color.isValid()) {
        ui_->mesDensityTable->item(row, column)->setBackgroundColor(color);
        preferenceManager_->updateMesosColorRange(0, color, row);
    }
}

void PreferenceDialog::updateMesosFlowColor(int row, int column) {
    // Only column 2 is color 'selectible'
    if (column != 2) return;
    QColorDialog colorPicker;
    QColor color(colorPicker.getColor(ui_->mesFlowTable->item(row, column)->backgroundColor(), this, tr("Choose Color"), QColorDialog::ShowAlphaChannel));
    if (color.isValid()) {
        ui_->mesFlowTable->item(row, column)->setBackgroundColor(color);
        preferenceManager_->updateMesosColorRange(1, color, row);
    }
}

void PreferenceDialog::updateMesosSpeedColor(int row, int column) {
    // Only column 2 is color 'selectible'
    if (column != 2) return;
    QColorDialog colorPicker;
    QColor color(colorPicker.getColor(ui_->mesSpeedTable->item(row, column)->backgroundColor(), this, tr("Choose Color"), QColorDialog::ShowAlphaChannel));
    if (color.isValid()) {
        ui_->mesSpeedTable->item(row, column)->setBackgroundColor(color);
        preferenceManager_->updateMesosColorRange(2, color, row);
    }
}

void PreferenceDialog::addNewMesosDensityRange() {
    RangeData *range = preferenceManager_->addNewMesosColorRange(0);
    int index = ui_->mesDensityTable->rowCount();
    ui_->mesDensityTable->insertRow(index);
    QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
    QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
    QTableWidgetItem *colorItem = new QTableWidgetItem();
    lowItem->setTextAlignment(Qt::AlignCenter);
    highItem->setTextAlignment(Qt::AlignCenter);
    colorItem->setFlags(Qt::ItemIsSelectable);
    colorItem->setBackgroundColor(range->getColor());
    ui_->mesDensityTable->setItem(index, 0, lowItem);
    ui_->mesDensityTable->setItem(index, 1, highItem);
    ui_->mesDensityTable->setItem(index, 2, colorItem);
}

void PreferenceDialog::addNewMesosFlowRange() {
    RangeData *range = preferenceManager_->addNewMesosColorRange(1);
    int index = ui_->mesFlowTable->rowCount();
    ui_->mesFlowTable->insertRow(index);
    QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
    QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
    QTableWidgetItem *colorItem = new QTableWidgetItem();
    lowItem->setTextAlignment(Qt::AlignCenter);
    highItem->setTextAlignment(Qt::AlignCenter);
    colorItem->setFlags(Qt::ItemIsSelectable);
    colorItem->setBackgroundColor(range->getColor());
    ui_->mesFlowTable->setItem(index, 0, lowItem);
    ui_->mesFlowTable->setItem(index, 1, highItem);
    ui_->mesFlowTable->setItem(index, 2, colorItem);
}

void PreferenceDialog::addNewMesosSpeedRange() {
    RangeData *range = preferenceManager_->addNewMesosColorRange(2);
    int index = ui_->mesSpeedTable->rowCount();
    ui_->mesSpeedTable->insertRow(index);
    QTableWidgetItem *lowItem = new QTableWidgetItem(QString::number(range->getLow(), 'f', 3));
    QTableWidgetItem *highItem = new QTableWidgetItem(QString::number(range->getHigh(), 'f', 3));
    QTableWidgetItem *colorItem = new QTableWidgetItem();
    lowItem->setTextAlignment(Qt::AlignCenter);
    highItem->setTextAlignment(Qt::AlignCenter);
    colorItem->setFlags(Qt::ItemIsSelectable);
    colorItem->setBackgroundColor(range->getColor());
    ui_->mesSpeedTable->setItem(index, 0, lowItem);
    ui_->mesSpeedTable->setItem(index, 1, highItem);
    ui_->mesSpeedTable->setItem(index, 2, colorItem);
}

void PreferenceDialog::editDensityRange(QTableWidgetItem *item) {
    int row = ui_->mesDensityTable->row(item);
    int column = ui_->mesDensityTable->column(item);
    double value = item->text().toDouble();
    if (column == 0)
        preferenceManager_->updateMesosLowValue(0, value, row);
    else
        preferenceManager_->updateMesosHighValue(0, value, row);
}

void PreferenceDialog::editFlowRange(QTableWidgetItem *item) {
    int row = ui_->mesFlowTable->row(item);
    int column = ui_->mesFlowTable->column(item);
    double value = item->text().toDouble();
    if (column == 0)
        preferenceManager_->updateMesosLowValue(1, value, row);
    else
        preferenceManager_->updateMesosHighValue(1, value, row);
}

void PreferenceDialog::editSpeedRange(QTableWidgetItem *item) {
    int row = ui_->mesSpeedTable->row(item);
    int column = ui_->mesSpeedTable->column(item);
    double value = item->text().toDouble();
    if (column == 0)
        preferenceManager_->updateMesosLowValue(2, value, row);
    else
        preferenceManager_->updateMesosHighValue(2, value, row);
}

void PreferenceDialog::removeSelectedDensityRange() {
    QList<QTableWidgetItem*> selectedItems = ui_->mesDensityTable->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        int row = ui_->mesDensityTable->row(selectedItems.at(i));
        if (row > -1) {
            ui_->mesDensityTable->removeRow(row);
            preferenceManager_->removeMesosColorRange(0, row);
        }
    }
}

void PreferenceDialog::removeSelectedFlowRange() {
    QList<QTableWidgetItem*> selectedItems = ui_->mesFlowTable->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        int row = selectedItems.at(i)->row();
        if (row > -1) {
            ui_->mesFlowTable->removeRow(row);
            preferenceManager_->removeMesosColorRange(1, row);
        }
    }
}

void PreferenceDialog::removeSelectedSpeedRange() {
    QList<QTableWidgetItem*> selectedItems = ui_->mesSpeedTable->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        int row = selectedItems.at(i)->row();
        if (row > -1) {
            ui_->mesSpeedTable->removeRow(row);
            preferenceManager_->removeMesosColorRange(2, row);
        }
    }
}
