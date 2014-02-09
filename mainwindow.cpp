#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    initData();
    ui_->setupUi(this);
    setWindowTitle("iSimGUI");
    initUi();
    connectSignalAction();
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete geospatialIndex_;
    delete temporalIndex_;
    delete fileReader_;
    delete preferenceManager_;
}

void MainWindow::initData() {
    preferenceManager_ = new PreferenceManager(this);
    geospatialIndex_ = new GeospatialIndex();
    temporalIndex_ = new TemporalIndex(this, preferenceManager_);
    viewController_ = new ViewController(geospatialIndex_, temporalIndex_, preferenceManager_);
    viewController_->start();
    fileReader_ = new FileReader(geospatialIndex_, temporalIndex_);
}

void MainWindow::initUi() {
    ui_->leftWidget->hide();
    //ui_->mapStackedWidget->hide();
    ui_->mapStackedWidget->setCurrentWidget(ui_->map);
    ui_->actionToggleBusRoute->setEnabled(false);
    ui_->actionToggleStaticElements->setEnabled(false);
    ui_->logTextEdit->setReadOnly(true);

    // add components to statusbar
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setVisible(false);
    pointerTracker = new QLabel(QString("x: %1; y: %2").arg(tr("00000000")).arg("00000000"));
    statusBar()->addPermanentWidget(progressBar);
    statusBar()->addPermanentWidget(pointerTracker);

    //map view
    scene_ = new QGraphicsScene(this);
    mapView_ = new MapGraphicsView(scene_, ui_->map, preferenceManager_);
    ui_->mapLayout->addWidget(mapView_);
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About iSimGUI"),
                       tr("The Integrated SimMobility Graphical User Interface, iSimGUI, "
                          "is a transportation simulation GUI application that can "
                          "visualize and edit different supported simulation formats "
                          "at the Driver, mesoscopic, and macroscopic levels."));
}

void MainWindow::open() {
    if (fileReader_->isRunning()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "A file is still loading, do you want to stop it and open another file ?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            fileReader_->stopReader();
        } else {
            return;
        }
    }

    OpenFileDialog fileDialog(this);
    fileDialog.setNameFilter(tr("ShortTerm Output Text File (*.txt);;MediumTerm Output Text File (*.txt);;SimMobility Input XML File (*.xml)"));
    fileDialog.addCheckBoxIn();
    fileDialog.show();
    if (fileDialog.exec() != QDialog::Accepted) return;

    QString path = fileDialog.selectedFiles().at(0);
    QString selectedFilter = fileDialog.selectedNameFilter();
    bool useDB = fileDialog.saveToDatabase->isChecked();
    iSimGUI::DataType type;
    if (selectedFilter.contains("ShortTerm")) type = iSimGUI::DATA_SHORT_TERM;
    else if (selectedFilter.contains("MediumTerm")) type = iSimGUI::DATA_MEDIUM_TERM;
    else type = iSimGUI::DATA_SIM_XML;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    fileReader_->wait();
    resetWorkspace();
    temporalIndex_->setUsingMemory();
    fileReader_->setTarget(path, type, useDB);
    fileReader_->start();
    progressBar->setVisible(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::hideLeftStackedWidget() {
    ui_->leftWidget->hide();
}

void MainWindow::addLog(QString log) {
    QTime time;
    ui_->logTextEdit->insertPlainText("["+time.currentTime().toString("hh:mm:ss.zzz")+"]" + log + "\n");
}

void MainWindow::saveLog() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Log"), "./mylog.txt", tr("Text (*.txt)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::critical(this, tr("iSim GUI"), tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        } else {
            // Write contents in textEdit widget into file.
            QTextStream out(&file);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            out << ui_->logTextEdit->toPlainText();
            file.close();
            QApplication::restoreOverrideCursor();
        }
    }
}

void MainWindow::alertFileError(QString error) {
    ui_->logTextEdit->insertPlainText(">> Loading file is aborted because " + error + "\n");
    QMessageBox::critical(this, "File Loading", "Loading file is aborted because " + error);
}

void MainWindow::openLogPage() {
    ui_->leftWidget->show();
    ui_->leftWidget->setWindowTitle("Log message");
    ui_->leftStackedWidget->setCurrentWidget(ui_->logWidget);
}

void MainWindow::openStaticElementsPage() {
    ui_->leftWidget->show();
    ui_->leftWidget->setWindowTitle("Map Static Elements");
    ui_->leftStackedWidget->setCurrentWidget(ui_->staticElementsWidget);
}

void MainWindow::openBusRoutePage() {
    ui_->leftWidget->show();
    ui_->leftWidget->setWindowTitle("Bus routes");
    ui_->leftStackedWidget->setCurrentWidget(ui_->busRouteWidget);
}

void MainWindow::updateProgressBar(int value) {
    progressBar->setValue(value);
}

void MainWindow::finishLoadFile() {
    progressBar->setVisible(false);
}

void MainWindow::connectSignalAction() {
    connect(ui_->actionAbout, SIGNAL(triggered()), this, SLOT(about()), Qt::QueuedConnection);
    connect(ui_->actionOpen, SIGNAL(triggered()), this, SLOT(open()), Qt::QueuedConnection);
    connect(ui_->actionToggleHideLeftWidget, SIGNAL(triggered()), this, SLOT(hideLeftStackedWidget()), Qt::QueuedConnection);
    connect(ui_->actionToggleStaticElements, SIGNAL(triggered()), this, SLOT(openStaticElementsPage()), Qt::QueuedConnection);
    connect(ui_->actionToggleBusRoute, SIGNAL(triggered()), this, SLOT(openBusRoutePage()), Qt::QueuedConnection);
    connect(ui_->actionToggleLog, SIGNAL(triggered()), this, SLOT(openLogPage()), Qt::QueuedConnection);
    connect(ui_->saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()), Qt::QueuedConnection);

    connect(mapView_, SIGNAL(announceMousePointerPosition(QPoint)), this, SLOT(updatePointerTracker(QPoint)), Qt::QueuedConnection);

    connect(fileReader_, SIGNAL(announceLog(QString)), this, SLOT(addLog(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceError(QString)), this, SLOT(alertFileError(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceProgressUpdated(int)), this, SLOT(updateProgressBar(int)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(finished()), this, SLOT(finishLoadFile()), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceStatus(QString)), statusBar(), SLOT(showMessage(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceSpatialDataFinished()), this, SLOT(loadGeospatial()), Qt::QueuedConnection);

    connect(viewController_, SIGNAL(requestCreateGNode(Node *)), this, SLOT(createGNode(Node *)), Qt::QueuedConnection);
}

void MainWindow::loadGeospatial() {
    // update some status & log
    addLog(tr("Finish parsing the geospatial elements, loading them to view."));
    statusBar()->showMessage(tr("Loading geospatial elements to view ..."));

    viewController_->addTask(iSimGUI::LOAD_NODE);

    addLog(tr("Finish loading geospatial elements to view ..."));
    statusBar()->showMessage(tr("Finish loading geospatial elements to view ..."));
}

void MainWindow::resetWorkspace() {
    scene_->clear();
    viewController_->reset();
    geospatialIndex_->reset();
    progressBar->setValue(0);
}

void MainWindow::createGNode(Node *node) {
    G_Node *gNode = new G_Node(0, node, preferenceManager_);
    scene_->addItem(gNode);
    viewController_->insertGNode(node->getId(), gNode);
}

void MainWindow::updatePointerTracker(QPoint point) {
    pointerTracker->setText(QString("x: %1; y: %2").arg(QString::number(point.x()).rightJustified(8, '0')).arg(QString::number(-point.y()).rightJustified(8, '0')));
}
