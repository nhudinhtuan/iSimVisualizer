#include "openfiledialog.h"

OpenFileDialog::OpenFileDialog(QWidget *parent) :
    QFileDialog(parent)
{
    this->setOption(QFileDialog::DontUseNativeDialog, true);
}

void OpenFileDialog::addCheckBoxIn() {
    QDialogButtonBox *box = this->findChild<QDialogButtonBox*>();
    QBoxLayout *l = box->findChild<QBoxLayout*>();
    saveToDatabase = new QCheckBox("Save To Database", box);
    //saveToDatabase->setChecked(true);
    l->insertWidget(0, saveToDatabase);
}
