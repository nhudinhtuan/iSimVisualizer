#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QFileDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QCheckBox>
#include <QButtonGroup>

class OpenFileDialog : public QFileDialog
{
public:
    OpenFileDialog(QWidget *parent);
    void addCheckBoxIn();

public:
    QCheckBox *saveToDatabase;
};

#endif // OPENFILEDIALOG_H
