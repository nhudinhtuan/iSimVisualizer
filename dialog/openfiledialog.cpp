#include "openfiledialog.h"

OpenFileDialog::OpenFileDialog(QWidget *parent) :
    QFileDialog(parent)
{
    this->setOption(QFileDialog::DontUseNativeDialog, true);
    this->setAcceptMode(QFileDialog::AcceptOpen);
}

void OpenFileDialog::customize(bool dbAvailable) {
    QGridLayout* mainLayout = this->findChild<QGridLayout*>();
    if (!mainLayout ) {
        // in case of future changes
    } else {
        // add some widgets
        QHBoxLayout *hbl = new QHBoxLayout(0);
        useMemory_ = new QRadioButton(tr("Use Memory (FAST)"));
        useDB_ = new QRadioButton(tr("Use DB temporarily"));
        keepInDB_ = new QRadioButton(tr("Save to DB for later use (SLOW)"));
        useMemory_->setChecked(true);
        hbl->addWidget(useMemory_);
        if (dbAvailable) {
            hbl->addWidget(useDB_);
            hbl->addWidget(keepInDB_);
        } else {
            QLabel *warning = new QLabel(0);
            warning->setText(tr("DB setting is not available! Please check Preference."));
            hbl->addWidget(warning);
        }
        int numRows = mainLayout->rowCount();
        mainLayout->addLayout(hbl, numRows, 0, 1, -1);
    }
}

iSimGUI::AccessType OpenFileDialog::getAccessOption() {
    if (useMemory_->isChecked())
        return iSimGUI::USE_MEMORY;
    if (useDB_->isChecked())
        return iSimGUI::USE_DB;
    return iSimGUI::KEEP_IN_DB;
}
