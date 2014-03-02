#ifndef OPENFILEDIALOG_H
#define OPENFILEDIALOG_H

#include <QFileDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QRadioButton>
#include <QHBoxLayout>

namespace iSimGUI {
    enum AccessType {USE_MEMORY = 0, USE_DB = 1, KEEP_IN_DB = 2};
}

class OpenFileDialog : public QFileDialog
{
public:
    OpenFileDialog(QWidget *parent);
    void customize();
    iSimGUI::AccessType getAccessOption();

private:
    QRadioButton *useMemory_;
    QRadioButton *useDB_;
    QRadioButton *keepInDB_;
};

#endif // OPENFILEDIALOG_H
