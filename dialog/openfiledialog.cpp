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
        useMemory_ = new QRadioButton(tr("Use Memory (FAST) - For small files."));
        useDB_ = new QRadioButton(tr("Use Database (SLOW) - For large files."));
        useMemory_->setChecked(true);
        hbl->addWidget(useMemory_);
        if (dbAvailable) {
            hbl->addWidget(useDB_);
        } else {
            QLabel *warning = new QLabel(0);
            warning->setText(tr("DB setting is not available! Please check Preference."));
            hbl->addWidget(warning);
        }
        int numRows = mainLayout->rowCount();
        mainLayout->addLayout(hbl, numRows, 0, 1, -1);
    }
}

//USE_MEMORY = 0, USE_DB = 1
int OpenFileDialog::getAccessOption() {
    if (useMemory_->isChecked())
        return 0;
    return 1;
}
