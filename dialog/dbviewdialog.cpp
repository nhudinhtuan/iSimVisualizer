#include "dbviewdialog.h"
#include "ui_dbviewdialog.h"
#define OPEN_ICON ":/ui_icons/open.png"
#define DELETE_ICON ":/ui_icons/delete.png"

DBViewDialog::DBViewDialog(QWidget *parent, DBManager *dbManager) :
    QDialog(parent),
    ui_(new Ui::DBViewDialog)
{
    ui_->setupUi(this);
    setWindowTitle(tr("Saved simmobility outputs."));
    setModal(true);
    dbManager_ = dbManager;

    ui_->recordTable->horizontalHeader()->resizeSection(0, 30);
    ui_->recordTable->horizontalHeader()->resizeSection(1, 30);
    ui_->recordTable->horizontalHeader()->resizeSection(2, 40);
    ui_->recordTable->horizontalHeader()->resizeSection(3, 50);
    ui_->recordTable->horizontalHeader()->resizeSection(4, 50);
    ui_->recordTable->horizontalHeader()->resizeSection(5, 60);
    ui_->recordTable->horizontalHeader()->resizeSection(6, 160);
    ui_->recordTable->verticalHeader()->setVisible(false);

    openMapper_ = new QSignalMapper(this);
    deleteMapper_ = new QSignalMapper(this);

    initSignal();
}

DBViewDialog::~DBViewDialog()
{
    delete ui_;
}

void DBViewDialog::initSignal() {
    connect(ui_->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(openMapper_, SIGNAL(mapped(int)), this, SLOT(openRecord(int)));
    connect(deleteMapper_, SIGNAL(mapped(int)), this, SLOT(deleteRecord(int)));
}

void DBViewDialog::loadRecords() {
    QList<Record> list = dbManager_->getAllRecords();
    ui_->recordTable->setRowCount(list.size());
    int count = 0;
    for (QList<Record>::iterator i = list.begin(); i != list.end(); ++i) {
        Record& item = *i;

        QPushButton *open = new QPushButton;
        open->setIcon(QIcon(OPEN_ICON));
        openMapper_->setMapping(open, item.id);
        connect(open, SIGNAL(clicked()), openMapper_, SLOT(map()));

        QPushButton *deleteBut = new QPushButton;
        deleteBut->setIcon(QIcon(DELETE_ICON));
        deleteMapper_->setMapping(deleteBut, item.id);
        connect(deleteBut, SIGNAL(clicked()), deleteMapper_, SLOT(map()));

        ui_->recordTable->setCellWidget(count, 0, open);
        ui_->recordTable->setCellWidget(count, 1, deleteBut);
        ui_->recordTable->setItem(count, 2, new QTableWidgetItem(QString::number(item.id)));
        QStringList type;
        if (item.isMacro) type << "macro";
        if (item.isMesos) type << "mesos";
        if (item.isMicro) type << "micro";
        ui_->recordTable->setItem(count, 3, new QTableWidgetItem(type.join(",")));
        ui_->recordTable->setItem(count, 4, new QTableWidgetItem(QString::number(item.size)));
        ui_->recordTable->setItem(count, 5, new QTableWidgetItem(QString::number(item.nframe)));
        ui_->recordTable->setItem(count, 6, new QTableWidgetItem(item.created));
        ui_->recordTable->setItem(count, 7, new QTableWidgetItem(item.path));
        count++;
    }
}

void DBViewDialog::deleteRecord(int id) {
    QMessageBox::StandardButton reply;
    QString warningMessage = "Are you sure to delete record " + QString::number(id);
    reply = QMessageBox::question(this, "Warning", warningMessage, QMessageBox::Yes|QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    dbManager_->removeRecord(id);
    loadRecords();
}

void DBViewDialog::openRecord(int id) {
    emit loadRecordFromDB(id);
    close();
}

