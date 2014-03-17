#ifndef DBVIEWDIALOG_H
#define DBVIEWDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <QDebug>
#include "io/dbmanager.h"

namespace Ui {
class DBViewDialog;
}

class DBViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DBViewDialog(QWidget *parent, DBManager *dbManager);
    virtual ~DBViewDialog();
    void loadRecords();

signals:
    void loadRecordFromDB(int id);

public slots:
    void deleteRecord(int id);
    void openRecord(int id);

private:
    void initSignal();

    Ui::DBViewDialog *ui_;
    DBManager *dbManager_;
    QSignalMapper *openMapper_;
    QSignalMapper *deleteMapper_;
};

#endif // DBVIEWDIALOG_H
