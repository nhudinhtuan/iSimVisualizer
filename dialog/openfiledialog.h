#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QLabel>

class OpenFileDialog : public QFileDialog
{
public:
    OpenFileDialog(QWidget *parent);
    void customize(bool dbAvailable);
    int getAccessOption();

private:
    QRadioButton *useMemory_;
    QRadioButton *useDB_;
};

#endif // OPENFILEDIALOG_H
