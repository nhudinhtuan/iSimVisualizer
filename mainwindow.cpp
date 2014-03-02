#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace iSimGUI {
    const double DEFAULT_ZOOM_LEVEL = 0.01;

    enum METACONTENT_TREE_ITEM {
        OTHERS = 0,
        UNINODE_TREE_TYPE = 1,
        MULTINODE_TREE_TYPE = 2,
        LINK_TREE_TYPE = 3,
        SEGMENT_TREE_TYPE = 4,
        LANE_TREE_TYPE = 5,
        CROSSING_TREE_TYPE = 6,
        BUSSTOP_TREE_TYPE = 7,
    };
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    setWindowTitle("iSimGUI");
    initData();
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
    delete mapView_;
    delete scene_;
}

void MainWindow::initData() {
    preferenceManager_ = new PreferenceManager(this);
    geospatialIndex_ = new GeospatialIndex();
    temporalIndex_ = new TemporalIndex(this, preferenceManager_);
    scene_ = new QGraphicsScene(this);
    mapView_ = new MapGraphicsView(scene_, ui_->map, preferenceManager_);
    viewController_ = new ViewController(geospatialIndex_, temporalIndex_, preferenceManager_, mapView_);
    viewController_->start();
    fileReader_ = new FileReader(geospatialIndex_, temporalIndex_);
    timer_ = new QTimer(this);
}

void MainWindow::initUi() {
    showMaximized();
    ui_->logTextEdit->setReadOnly(true);
    // add components to statusbar
    progressBar_ = new QProgressBar(this);
    progressBar_->setRange(0, 100);
    progressBar_->setVisible(false);
    pointerTracker_ = new QLabel(QString("x: %1; y: %2").arg(tr("00000000")).arg("00000000"));
    statusBar()->addPermanentWidget(progressBar_);
    statusBar()->addPermanentWidget(pointerTracker_);

    // add map view
    ui_->mapLayout->addWidget(mapView_);

    // init tree items
#if QT_VERSION >= 0x050000
    ui_->geospatialTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui_->geospatialTree->header()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    ui_->geospatialTree->setAllColumnsShowFocus(true);
    uninodeTreeItems_ = 0;
    multinodeTreeItems_ = 0;
    linkLabelTreeItems_ = 0;
    busStopTreeItems_ = 0;
    crossingTreeItems_ = 0;

    // properties table
    ui_->propertiesTable->horizontalHeader()->setStretchLastSection(true);
    ui_->propertiesTable->horizontalHeader()->resizeSection(0, 130);
    ui_->propertiesTable->verticalHeader()->setVisible(false);
    ui_->geoSplitter->setStretchFactor(0, 1);
    ui_->geoSplitter->setStretchFactor(1, 0);
    resetUi();
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
    /*
    iSimGUI::DataType type;
    if (selectedFilter.contains("ShortTerm")) type = iSimGUI::DATA_SHORT_TERM;
    else if (selectedFilter.contains("MediumTerm")) type = iSimGUI::DATA_MEDIUM_TERM;
    else type = iSimGUI::DATA_SIM_XML;*/

    QApplication::setOverrideCursor(Qt::WaitCursor);
    fileReader_->wait();
    resetWorkspace();
    temporalIndex_->setUsingMemory();
    fileReader_->setTarget(path);
    fileReader_->start();
    progressBar_->setVisible(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::hideLeftStackedWidget() {
    ui_->leftWidget->hide();
}

void MainWindow::addLog(QString log) {
    QTime time;
    ui_->logTextEdit->insertPlainText(time.currentTime().toString("hh:mm:ss.zzz")+" > " + log + "\n");
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
    ui_->leftWidget->setWindowTitle("Log Message");
    ui_->leftStackedWidget->setCurrentWidget(ui_->logWidget);
}

void MainWindow::openGeospatialElementsPage() {
    ui_->leftWidget->show();
    ui_->leftWidget->setWindowTitle("Geospatial Elements Tree");
    ui_->leftStackedWidget->setCurrentWidget(ui_->geospatialWidget);
}

void MainWindow::openBusRoutePage() {
    ui_->leftWidget->show();
    ui_->leftWidget->setWindowTitle("Bus Routes");
    ui_->leftStackedWidget->setCurrentWidget(ui_->busRouteWidget);
}

void MainWindow::findLocation() {
    bool okClicked;
    QString text = QInputDialog::getText(this, tr("Find Location"),
                                         tr("Please enter the location to focus on : "), QLineEdit::Normal,
                                         "<xPos>, <yPos>", &okClicked);
    if (okClicked && !text.isEmpty()) {
        QStringList coordinates = text.split(",", QString::SkipEmptyParts);
        if (coordinates.size() == 2)
            mapView_->centerOn(coordinates.at(0).toLong(), -coordinates.at(1).toLong());
    }
}

void MainWindow::openPreferencesDialog() {
    pauseSimulation();
    PreferenceDialog preDialog(this, preferenceManager_);
    preDialog.show();
    preDialog.exec();
}

void MainWindow::updateMapView() {
    mapView_->viewport()->update();
}

void MainWindow::updateProgressBar(int value) {
    progressBar_->setValue(value);
}

void MainWindow::finishLoadFile() {
    progressBar_->setVisible(false);
}

void MainWindow::connectSignalAction() {
    connect(ui_->actionAbout, SIGNAL(triggered()), this, SLOT(about()), Qt::QueuedConnection);
    connect(ui_->actionOpen, SIGNAL(triggered()), this, SLOT(open()), Qt::QueuedConnection);
    connect(ui_->actionToggleHideLeftWidget, SIGNAL(triggered()), this, SLOT(hideLeftStackedWidget()), Qt::QueuedConnection);
    connect(ui_->actionToggleStaticElements, SIGNAL(triggered()), this, SLOT(openGeospatialElementsPage()), Qt::QueuedConnection);
    connect(ui_->actionToggleBusRoute, SIGNAL(triggered()), this, SLOT(openBusRoutePage()), Qt::QueuedConnection);
    connect(ui_->actionToggleLog, SIGNAL(triggered()), this, SLOT(openLogPage()), Qt::QueuedConnection);
    connect(ui_->actionTogglePointTracker, SIGNAL(triggered()), this, SLOT(findLocation()), Qt::QueuedConnection);
    connect(ui_->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferencesDialog()), Qt::QueuedConnection);
    connect(ui_->saveLogButton, SIGNAL(clicked()), this, SLOT(saveLog()), Qt::QueuedConnection);
    connect(ui_->geospatialTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(focusOnGraphicsOfTreeItem(QTreeWidgetItem*, int)));
    connect(ui_->geospatialSearchField, SIGNAL(textChanged(QString)), this, SLOT(searchGeo(QString)));
    connect(ui_->startSim, SIGNAL(clicked()), this, SLOT(startSimulation()), Qt::QueuedConnection);
    connect(ui_->pauseSim, SIGNAL(clicked()), this, SLOT(pauseSimulation()), Qt::QueuedConnection);
    connect(ui_->spinTick, SIGNAL(valueChanged(int)), ui_->sliderTick, SLOT(setValue(int)), Qt::QueuedConnection);
    connect(ui_->sliderTick, SIGNAL(valueChanged(int)), ui_->spinTick, SLOT(setValue(int)), Qt::QueuedConnection);
    connect(ui_->sliderTick, SIGNAL(valueChanged(int)), this, SLOT(jumpToSimulation(int)), Qt::QueuedConnection);
    connect(ui_->sliderTick, SIGNAL(sliderPressed()), this, SLOT(pauseSimulation()), Qt::QueuedConnection);
    connect(timer_, SIGNAL(timeout()), this, SLOT(jumpToNextTick()), Qt::QueuedConnection);

    connect(mapView_, SIGNAL(announceMousePointerPosition(QPoint)), this, SLOT(updatePointerTracker(QPoint)), Qt::QueuedConnection);
    connect(mapView_, SIGNAL(announceMapviewScroll()), this, SLOT(requestUpdateDynamicData()));

    connect(preferenceManager_, SIGNAL(updateBgColor()), mapView_, SLOT(updateBackgroundColor()), Qt::QueuedConnection);
    connect(preferenceManager_, SIGNAL(updateMapViewAttr()), this, SLOT(updateMapView()), Qt::QueuedConnection);
    connect(preferenceManager_, SIGNAL(updateAgents()), this, SLOT(requestUpdateAgents()), Qt::QueuedConnection);
    connect(preferenceManager_, SIGNAL(updateMicroData()), this, SLOT(requestUpdateMicroData()), Qt::QueuedConnection);

    connect(fileReader_, SIGNAL(announceLog(QString)), this, SLOT(addLog(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceError(QString)), this, SLOT(alertFileError(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceProgressUpdated(int)), this, SLOT(updateProgressBar(int)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(finished()), this, SLOT(finishLoadFile()), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceStatus(QString)), statusBar(), SLOT(showMessage(QString)), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceSpatialDataFinished()), this, SLOT(loadGeospatial()), Qt::QueuedConnection);
    connect(fileReader_, SIGNAL(announceTemporalDataExists()), this, SLOT(showSimulationGUI()), Qt::QueuedConnection);

    connect(viewController_, SIGNAL(requestCreateGUniNode(UniNode *)), this, SLOT(createGUniNode(UniNode *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGMultiNode(MultiNode *)), this, SLOT(createGMultiNode(MultiNode *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGBusStop(BusStop *)), this, SLOT(createGBusStop(BusStop *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGSegment(RoadSegment *)), this, SLOT(createGSegment(RoadSegment *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGLane(Lane *)), this, SLOT(createGLane(Lane *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGLaneConnector(LaneConnector*)), this, SLOT(createGLaneConnector(LaneConnector*)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGCrossing(Crossing*)), this, SLOT(createGCrossing(Crossing*)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateGTrafficSignal(TrafficSignal*)), this, SLOT(createGTrafficSignal(TrafficSignal*)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestCreateLinkTreeItem(Link *)), this, SLOT(createLinkTreeItem(Link *)), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(finishLoadingGeospatial()), this, SLOT(finishLoadingGeospatial()), Qt::QueuedConnection);
    connect(viewController_, SIGNAL(requestUpdateGAgents(AgentList*)), this, SLOT(updateGAgents(AgentList*)), Qt::QueuedConnection);

    connect(temporalIndex_, SIGNAL(announceNewUpperTickValue(unsigned int)), this, SLOT(updateUpperTickValue(unsigned int)), Qt::QueuedConnection);
}

void MainWindow::loadGeospatial() {
    // update some status & log
    addLog(tr("Finish parsing the geospatial elements, loading them to view."));
    statusBar()->showMessage(tr("Loading geospatial elements to view ..."));

    viewController_->addTask(iSimGUI::LOAD_GEOSPATIAL);
}

void MainWindow::finishLoadingGeospatial() {
    addLog(tr("Finish loading geospatial elements to view ..."));
    statusBar()->showMessage(tr("Finish loading geospatial elements to view ..."));

    QRectF scenceRect = scene_->sceneRect();
    qreal x, y, w, h;
    scenceRect.getRect(&x, &y, &w, &h);
    mapView_->setZoomFactor(iSimGUI::DEFAULT_ZOOM_LEVEL*1000);
    mapView_->scale(iSimGUI::DEFAULT_ZOOM_LEVEL, iSimGUI::DEFAULT_ZOOM_LEVEL);

    // set new scence rect
    x -= w/2;
    y -= h/2;
    w += w;
    h += h;
    scene_->setSceneRect(x, y, w, h);

    mapView_->centerOn(x + w/2, y + h/2);

    // enable UI components
    ui_->mapStackedWidget->show();
    ui_->actionToggleBusRoute->setEnabled(true);
    ui_->actionToggleStaticElements->setEnabled(true);
    ui_->actionTogglePointTracker->setEnabled(true);
}

void MainWindow::resetWorkspace() {
    pauseSimulation();
    if(scene_) delete scene_;
    scene_ = new QGraphicsScene(this);
    scene_->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(updateSelectedTreeItem()), Qt::QueuedConnection);
    mapView_->setScene(scene_);
    mapView_->resetMatrix();

    viewController_->reset();
    geospatialIndex_->reset();
    temporalIndex_->reset();
    progressBar_->setValue(0);
    resetUi();
}

void MainWindow::resetUi() {
    ui_->leftWidget->hide();
    ui_->mapStackedWidget->hide();
    ui_->mapStackedWidget->setCurrentWidget(ui_->map);
    ui_->spinTick->hide();
    ui_->labelTickUnit->hide();
    ui_->sliderTick->hide();
    ui_->startSim->hide();
    ui_->pauseSim->hide();
    ui_->labelUpperBoundTick->hide();
    ui_->actionToggleBusRoute->setEnabled(false);
    ui_->actionToggleStaticElements->setEnabled(false);
    ui_->actionTogglePointTracker->setEnabled(false);

    if (uninodeTreeItems_) delete uninodeTreeItems_;
    uninodeTreeItems_ = new QTreeWidgetItem(ui_->geospatialTree, QStringList("UNINODES "));
    uninodeTreeItems_->setData(0, Qt::UserRole, iSimGUI::OTHERS);
    uninodeTreeItems_->setText(1, tr("%n item(s)", "", uninodeTreeItems_->childCount()));
    if (multinodeTreeItems_) delete multinodeTreeItems_;
    multinodeTreeItems_ = new QTreeWidgetItem(ui_->geospatialTree, QStringList("MULTINODES "));
    multinodeTreeItems_->setData(0, Qt::UserRole, iSimGUI::OTHERS);
    if (linkLabelTreeItems_) delete linkLabelTreeItems_;
    linkLabelTreeItems_ = new QTreeWidgetItem(ui_->geospatialTree, QStringList("LINKS "));
    linkLabelTreeItems_->setData(0, Qt::UserRole, iSimGUI::OTHERS);
    if (crossingTreeItems_) delete crossingTreeItems_;
    crossingTreeItems_ = new QTreeWidgetItem(ui_->geospatialTree, QStringList("CROSSING "));
    crossingTreeItems_->setData(0, Qt::UserRole, iSimGUI::OTHERS);
    if (busStopTreeItems_) delete busStopTreeItems_;
    busStopTreeItems_ = new QTreeWidgetItem(ui_->geospatialTree, QStringList("BUSSTOPS "));
    busStopTreeItems_->setData(0, Qt::UserRole, iSimGUI::OTHERS);
}

void MainWindow::showSimulationGUI() {
    ui_->spinTick->show();
    ui_->labelTickUnit->show();
    ui_->sliderTick->show();
    ui_->startSim->show();
    ui_->pauseSim->hide();
    ui_->labelUpperBoundTick->show();


    ui_->sliderTick->setMaximum(0);
    ui_->sliderTick->setValue(0);
    ui_->spinTick->setMaximum(0);
    ui_->spinTick->setValue(0);


}

void MainWindow::updateUpperTickValue(unsigned int value) {
    ui_->sliderTick->setMaximum(value);
    ui_->spinTick->setMaximum(value);
    ui_->labelUpperBoundTick->setText(QString("/%1").arg(value));
}

void MainWindow::updatePointerTracker(QPoint point) {
    pointerTracker_->setText(QString("x: %1; y: %2").arg(QString::number(point.x()).rightJustified(8, '0')).arg(QString::number(-point.y()).rightJustified(8, '0')));
}

void MainWindow::createGUniNode(UniNode *uniNode) {
    G_Node *gNode = new G_UniNode(0, uniNode, preferenceManager_, mapView_);
    scene_->addItem(gNode);

    // add tree item
    QTreeWidgetItem *item = new QTreeWidgetItem(uninodeTreeItems_, QStringList() << QString("%1 [aimsun-%2]").arg(uniNode->getId())
                                                    .arg(uniNode->getAimsunId()), iSimGUI::UNINODE_TREE_TYPE);
    QVariant variantId = (qulonglong)uniNode->getId();
    QVariant variantGNode = qVariantFromValue(gNode);
    item->setData(0, Qt::UserRole, variantId);
    item->setData(1, Qt::UserRole, variantGNode);
    uninodeTreeItems_->setText(1, tr("%n item(s)", "", uninodeTreeItems_->childCount()));
}

void MainWindow::createGMultiNode(MultiNode *multiNode) {
    G_Node *gNode = new G_MultiNode(0, multiNode, preferenceManager_, mapView_);
    scene_->addItem(gNode);

    // add tree item
    QTreeWidgetItem *item = new QTreeWidgetItem(multinodeTreeItems_, QStringList() << QString("%1 [aimsun-%2]").arg(multiNode->getId())
                                                    .arg(multiNode->getAimsunId()), iSimGUI::MULTINODE_TREE_TYPE);
    QVariant variantId = (qulonglong)multiNode->getId();
    QVariant variantGNode = qVariantFromValue(gNode);
    item->setData(0, Qt::UserRole, variantId);
    item->setData(1, Qt::UserRole, variantGNode);
    multinodeTreeItems_->setText(1, tr("%n item(s)", "", multinodeTreeItems_->childCount()));
}

void MainWindow::createGBusStop(BusStop *busStop) {
    G_BusStop *gBusStop = new G_BusStop(0, busStop, preferenceManager_, mapView_);
    scene_->addItem(gBusStop);

    // add tree item
    QTreeWidgetItem *item = new QTreeWidgetItem(busStopTreeItems_, QStringList() << QString("%1").arg(busStop->getId()), iSimGUI::BUSSTOP_TREE_TYPE);
    QVariant variantId = (qulonglong)busStop->getId();
    QVariant variantGBusStop = qVariantFromValue(gBusStop);
    item->setData(0, Qt::UserRole, variantId);
    item->setData(1, Qt::UserRole, variantGBusStop);
    busStopTreeItems_->setText(1, tr("%n item(s)", "", busStopTreeItems_->childCount()));
}

void MainWindow::createLinkTreeItem(Link *link) {
    QTreeWidgetItem *linkTreeItem = new QTreeWidgetItem(linkLabelTreeItems_, QStringList() << QString("%1").arg(link->getId()), iSimGUI::LINK_TREE_TYPE);
    QVariant variantId = (qulonglong)link->getId();
    linkTreeItem->setData(0, Qt::UserRole, variantId);
    linkLabelTreeItems_->setText(1, tr("%n item(s)", "", linkLabelTreeItems_->childCount()));
    linkTreeItems_[link->getId()] = linkTreeItem;
}

void MainWindow::createGSegment(RoadSegment *segment) {
    G_Segment *gSegment = new G_Segment(0, segment, preferenceManager_, mapView_);
    scene_->addItem(gSegment);

    // add tree item
    if (linkTreeItems_.contains(segment->getlinkId())) {
        QTreeWidgetItem *linkTreeItem = linkTreeItems_[segment->getlinkId()];

        QTreeWidgetItem *segmentItem = new QTreeWidgetItem(linkTreeItem, QStringList() << QString("SEG %1").arg(segment->getId()), iSimGUI::SEGMENT_TREE_TYPE);
        QVariant variantId = (qulonglong)segment->getId();
        QVariant variantGSegment = qVariantFromValue(gSegment);
        segmentItem->setData(0, Qt::UserRole, variantId);
        segmentItem->setData(1, Qt::UserRole, variantGSegment);
        segmentItems_[segment->getId()] = segmentItem;
    }
}

void MainWindow::createGLane(Lane *lane) {
    G_Lane *gLane = new G_Lane(0, lane, preferenceManager_, mapView_);
    scene_->addItem(gLane);

    // add tree item
    if (segmentItems_.contains(lane->getSegmentId())) {
        QTreeWidgetItem *segmentTreeItem = segmentItems_[lane->getSegmentId()];
        QTreeWidgetItem *laneItem = new QTreeWidgetItem(segmentTreeItem, QStringList() << QString("LANE %1").arg(lane->getIndex()), iSimGUI::LANE_TREE_TYPE);
        QVariant variantId = (uint)lane->getIndex();
        QVariant variantGLane = qVariantFromValue(gLane);
        laneItem->setData(0, Qt::UserRole, variantId);
        laneItem->setData(1, Qt::UserRole, variantGLane);
    }
}

void MainWindow::createGLaneConnector(LaneConnector *laneConnector) {
    G_LaneConnector* gLaneConnector = new G_LaneConnector(0, laneConnector, preferenceManager_, mapView_);
    scene_->addItem(gLaneConnector);
}

void MainWindow::createGCrossing(Crossing *crossing) {
    G_Crossing *gCrossing = new G_Crossing(0, crossing, preferenceManager_, mapView_, temporalIndex_);
    scene_->addItem(gCrossing);

    // add tree item
    QTreeWidgetItem *item = new QTreeWidgetItem(crossingTreeItems_, QStringList() << QString("%1").arg(crossing->getId()), iSimGUI::CROSSING_TREE_TYPE);
    QVariant variantId = (qulonglong)crossing->getId();
    QVariant variantGCrossing = qVariantFromValue(gCrossing);
    item->setData(0, Qt::UserRole, variantId);
    item->setData(1, Qt::UserRole, variantGCrossing);
    crossingTreeItems_->setText(1, tr("%n item(s)", "", crossingTreeItems_->childCount()));
}

void MainWindow::createGTrafficSignal(TrafficSignal *trafficSignal) {
    G_TrafficSignal* gTrafficSignal = new G_TrafficSignal(0, trafficSignal, preferenceManager_, mapView_, temporalIndex_);
    scene_->addItem(gTrafficSignal);
}

void MainWindow::updateGAgents(AgentList* agents) {
    QSet<unsigned long> seenItems;
    if (agents) {
        QList<Agent*>& list = agents->getList();
        for (QList<Agent*>::iterator agentsIt = list.begin(); agentsIt != list.end(); ++agentsIt) {
            Agent* agent = *agentsIt;
            seenItems.insert(agent->getID());
            if (gAgents_.contains(agent->getID())) {
                G_Agent* currentAgent = gAgents_[agent->getID()];
                if (currentAgent->getType() == agent->getType()) {
                    currentAgent->updateModel(agent);
                    continue;
                } else {
                    scene_->removeItem(currentAgent);
                    delete currentAgent;
                }
            }
            G_Agent* gAgent = G_AgentFactory::create(agent, preferenceManager_, mapView_);
            if (gAgent) {
                scene_->addItem(gAgent);
                gAgents_[agent->getID()] = gAgent;
            }
        }
        delete agents;
    }
    QHash<unsigned long, G_Agent*>::iterator gAgentIt = gAgents_.begin();
    while (gAgentIt != gAgents_.end()) {
        if (!seenItems.contains(gAgentIt.key())) {
            scene_->removeItem(gAgentIt.value());
            delete gAgentIt.value();
            gAgentIt = gAgents_.erase(gAgentIt);
        } else {
            gAgentIt++;
        }
    }

    updateMapView();
}

void MainWindow::focusOnGraphicsOfTreeItem(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column)

    // show attributes

    scene_->clearSelection();
    QGraphicsItem *gItem = 0;
    switch (item->type()) {
        case iSimGUI::MULTINODE_TREE_TYPE:
        case iSimGUI::UNINODE_TREE_TYPE: {
            QVariant gData = item->data(1, Qt::UserRole);
            G_Node *gNode = gData.value<G_Node*>();
            gItem = gNode;
            showNodeProperty(gNode->getModel());
            break;
        }
        case iSimGUI::LINK_TREE_TYPE: {
            QVariant linkId = item->data(0, Qt::UserRole);
            Link *data = geospatialIndex_->getLink(linkId.toULongLong());
            showLinkProperty(data);
            break;
        }
        case iSimGUI::SEGMENT_TREE_TYPE: {
            QVariant gData = item->data(1, Qt::UserRole);
            G_Segment *gSeg = gData.value<G_Segment*>();
            gItem = gSeg;
            showSegmentProperty(gSeg->getModel());
            break;
        }
        case iSimGUI::LANE_TREE_TYPE: {
            QVariant gData = item->data(1, Qt::UserRole);
            G_Lane *gLane = gData.value<G_Lane*>();
            gItem = gLane;
            showLaneProperty(gLane->getModel());
            break;
        }
        case iSimGUI::CROSSING_TREE_TYPE: {
            QVariant gData = item->data(1, Qt::UserRole);
            G_Crossing *gCrossing = gData.value<G_Crossing*>();
            gItem = gCrossing;
            showCrossingProperty(gCrossing->getModel());
            break;
        }
        case iSimGUI::BUSSTOP_TREE_TYPE: {
            QVariant gData = item->data(1, Qt::UserRole);
            G_BusStop *gBusStop = gData.value<G_BusStop*>();
            gItem = gBusStop;
            showBusStopProperty(gBusStop->getModel());
            break;
        }
    }
    if (gItem) {
        gItem->setSelected(true);
        mapView_->centerOn(gItem);
    }
}

void MainWindow::updateSelectedTreeItem() {
    QList<QGraphicsItem*> selectedItems = scene_->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    QTreeWidgetItem *treeItem = getTreeItemFromGraphics(selectedItems.first());
    if (treeItem) {
        treeItem->setHidden(false);
        ui_->geospatialTree->setCurrentItem(treeItem);
    }
}

QTreeWidgetItem* MainWindow::getTreeItemFromGraphics(QGraphicsItem *gItem) {
    G_Node *gNode = dynamic_cast<G_Node*>(gItem);
    if (gNode) {
        QTreeWidgetItem *parent = gNode->getNodeType() == iSimGUI::UNI_NODE ? uninodeTreeItems_ : multinodeTreeItems_;
        for(int i = 0; i < parent->childCount(); ++i) {
            QTreeWidgetItem* child = parent->child(i);
            QVariant variantId = child->data(0, Qt::UserRole);
            if (gNode->getModelId() == variantId.toULongLong()) {
                showNodeProperty(gNode->getModel());
                openGeospatialElementsPage();
                return child;
            }
        }
        return 0;
    }

    G_BusStop *gBusstop = dynamic_cast<G_BusStop*>(gItem);
    if (gBusstop) {
        for(int i = 0; i < busStopTreeItems_->childCount(); ++i) {
            QTreeWidgetItem* child = busStopTreeItems_->child(i);
            QVariant variantId = child->data(0, Qt::UserRole);
            if (gBusstop->getModelId() == variantId.toULongLong()) {
                showBusStopProperty(gBusstop->getModel());
                openGeospatialElementsPage();
                return child;
            }
        }
        return 0;
    }

    G_Segment *gSegment = dynamic_cast<G_Segment*>(gItem);
    if (gSegment) {
        showSegmentProperty(gSegment->getModel());
        openGeospatialElementsPage();
        return segmentItems_[gSegment->getSegmentId()];
    }

    G_Lane *gLane = dynamic_cast<G_Lane*>(gItem);
    if (gLane) {
        QTreeWidgetItem *segmentTreeItems = segmentItems_[gLane->getSegmentId()];
        for(int i = 0; i < segmentTreeItems->childCount(); ++i) {
            QTreeWidgetItem* child = segmentTreeItems->child(i);
            QVariant variantId = child->data(0, Qt::UserRole);
            if (gLane->getLaneIndex() == variantId.toUInt()) {
                showLaneProperty(gLane->getModel());
                openGeospatialElementsPage();
                return child;
            }
        }
        return 0;
    }

    G_Crossing *gCrossing = dynamic_cast<G_Crossing*>(gItem);
    if (gCrossing) {
        for(int i = 0; i < crossingTreeItems_->childCount(); ++i) {
            QTreeWidgetItem* child = crossingTreeItems_->child(i);
            QVariant variantId = child->data(0, Qt::UserRole);
            if (gCrossing->getId() == variantId.toULongLong()) {
                showCrossingProperty(gCrossing->getModel());
                openGeospatialElementsPage();
                return child;
            }
        }
        return 0;
    }
    return 0;
}

void MainWindow::showNodeProperty(Node *data) {
    ui_->propertiesTable->setRowCount(4);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("Node ID "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getId())));
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("Aimsun ID "));
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(tr("%1").arg(data->getAimsunId())));
    QPointF& pos = data->getPos();
    ui_->propertiesTable->setItem(2, 0, new QTableWidgetItem("XPos "));
    ui_->propertiesTable->setItem(2, 1, new QTableWidgetItem(tr("%1").arg(pos.x())));
    ui_->propertiesTable->setItem(3, 0, new QTableWidgetItem("YPos "));
    ui_->propertiesTable->setItem(3, 1, new QTableWidgetItem(tr("%1").arg(-pos.y())));
}

void MainWindow::showLinkProperty(Link *data) {
    ui_->propertiesTable->setRowCount(4);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("ID "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getId())));
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("Name "));
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(data->getName()));
    ui_->propertiesTable->setItem(2, 0, new QTableWidgetItem("Start Node Id "));
    ui_->propertiesTable->setItem(2, 1, new QTableWidgetItem(tr("%1").arg(data->getStartNodeId())));
    ui_->propertiesTable->setItem(3, 0, new QTableWidgetItem("End Node Id "));
    ui_->propertiesTable->setItem(3, 1, new QTableWidgetItem(tr("%1").arg(data->getEndNodeId())));
}


void MainWindow::showSegmentProperty(RoadSegment *data) {
    ui_->propertiesTable->setRowCount(7);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("ID "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getId())));
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("Aimsun ID "));
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(tr("%1").arg(data->getAimsunId())));
    ui_->propertiesTable->setItem(2, 0, new QTableWidgetItem("From Node Id "));
    ui_->propertiesTable->setItem(2, 1, new QTableWidgetItem(tr("%1").arg(data->getFromNodeId())));
    ui_->propertiesTable->setItem(3, 0, new QTableWidgetItem("To Node Id "));
    ui_->propertiesTable->setItem(3, 1, new QTableWidgetItem(tr("%1").arg(data->getToNodeId())));
    ui_->propertiesTable->setItem(4, 0, new QTableWidgetItem("Max Speed "));
    ui_->propertiesTable->setItem(4, 1, new QTableWidgetItem(tr("%1").arg(data->getMaxSpeed())));
    ui_->propertiesTable->setItem(5, 0, new QTableWidgetItem("Width "));
    ui_->propertiesTable->setItem(5, 1, new QTableWidgetItem(tr("%1").arg(data->getWidth())));
    ui_->propertiesTable->setItem(6, 0, new QTableWidgetItem("Number Lane "));
    ui_->propertiesTable->setItem(6, 1, new QTableWidgetItem(tr("%1").arg(data->getNLane())));
}

void MainWindow::showLaneProperty(Lane *data) {
    ui_->propertiesTable->setRowCount(3);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("Index "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getIndex())));
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("Is Side Walk "));
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(tr("%1").arg(data->isSideWalk())));
    ui_->propertiesTable->setItem(2, 0, new QTableWidgetItem("Polyline "));
    QStringList polyline;
    QVector<QPointF>& points = data->getPolyline();
    for (int i = 0; i < points.size(); ++i) {
        QPointF p = points.at(i);
        polyline << QString("(%1, %2)").arg(QString::number(p.x())).arg(QString::number(-p.y()));
    }
    ui_->propertiesTable->setItem(2, 1, new QTableWidgetItem(polyline.join(" , ")));
}

void MainWindow::showCrossingProperty(Crossing *data) {
    ui_->propertiesTable->setRowCount(2);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("Crossing ID "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getId())));
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("Polyline "));
    QStringList polyline;
    QVector<QPointF>& points = data->getPolyline();
    for (int i = 0; i < points.size(); ++i) {
        QPointF p = points.at(i);
        polyline << QString("(%1, %2)").arg(QString::number(p.x())).arg(QString::number(-p.y()));
    }
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(polyline.join(" , ")));
}

void MainWindow::showBusStopProperty(BusStop *data) {
    ui_->propertiesTable->setRowCount(3);
    ui_->propertiesTable->setItem(0, 0, new QTableWidgetItem("Node ID "));
    ui_->propertiesTable->setItem(0, 1, new QTableWidgetItem(tr("%1").arg(data->getId())));
    QPointF& pos = data->getPos();
    ui_->propertiesTable->setItem(1, 0, new QTableWidgetItem("XPos "));
    ui_->propertiesTable->setItem(1, 1, new QTableWidgetItem(tr("%1").arg(pos.x())));
    ui_->propertiesTable->setItem(2, 0, new QTableWidgetItem("YPos "));
    ui_->propertiesTable->setItem(2, 1, new QTableWidgetItem(tr("%1").arg(-pos.y())));
}

void MainWindow::searchGeo(QString key) {

    // back to default
    for (int i = 0; i < ui_->geospatialTree->topLevelItemCount(); i++) {
        QTreeWidgetItem *child = ui_->geospatialTree->topLevelItem(i);
        child->setHidden(false);
        child->setExpanded(false);
        child->setBackgroundColor(0, Qt::white);
        child->setBackgroundColor(1, Qt::white);
        setGeoTreeItemsToDefaultState(child);
    }

    if (key.isEmpty()) return;
    QList<QTreeWidgetItem*> searchResults = ui_->geospatialTree->findItems(key, Qt::MatchContains|Qt::MatchRecursive, 0);
    filterGeoTreeItems(searchResults);
}


void MainWindow::setGeoTreeItemsToDefaultState(QTreeWidgetItem *item) {
    for (int i = 0; i < item->childCount(); i++) {
        QTreeWidgetItem *child = item->child(i);
        child->setHidden(false);
        child->setExpanded(false);
        child->setBackgroundColor(0, Qt::white);
        child->setBackgroundColor(1, Qt::white);
        setGeoTreeItemsToDefaultState(child);
    }
}

void MainWindow::filterGeoTreeItems(QList<QTreeWidgetItem *> &items) {
    // filter
    for (int i = 0; i < ui_->geospatialTree->topLevelItemCount(); i++) {
        // (1) If the child is a target item, leave the descendants as visible i.e. no need to do anything
        // (2) If it is not a target item, we check if all of its descendants are also not target items
        // (3) If (2) is true, we set the child to be hidden
        QTreeWidgetItem *topLevelItem = ui_->geospatialTree->topLevelItem(i);
        if (!items.contains(topLevelItem)) {
            if (!filterGeoTreeItemsRecursive(items, topLevelItem)) {
                topLevelItem->setHidden(true);
            }
            topLevelItem->setExpanded(true);
            topLevelItem->setBackgroundColor(0,  QColor(Qt::white));
            topLevelItem->setBackgroundColor(1,  QColor(Qt::white));
        } else {
            topLevelItem->setExpanded(false);
            topLevelItem->setBackgroundColor(0,  QColor(Qt::gray));
            topLevelItem->setBackgroundColor(1,  QColor(Qt::gray));
        }
    }
}

bool MainWindow::filterGeoTreeItemsRecursive(QList<QTreeWidgetItem *> &items, QTreeWidgetItem* parent) {
    bool isDescendantA_Target = false;

    for (int i = 0; i < parent->childCount(); i++) {
        // (1) If the child is a target item, leave the descendants as visible i.e. no need to do anything
        // (2) If it is not a target item, we check if all of its descendants are also not target items
        // (3) If (2) is true, we set the child to be hidden
        // (4) We return a true if at least one of the children is a target descendant
        QTreeWidgetItem *child = parent->child(i);
        if (!items.contains((child)) ) {
            if (!filterGeoTreeItemsRecursive(items, child)) {
                child->setHidden(true);
            } else {
                isDescendantA_Target |= true;
            }
            child->setExpanded(true);
            child->setBackgroundColor(0, QColor(Qt::white));
            child->setBackgroundColor(1, QColor(Qt::white));
        } else {
            child->setExpanded(false);
            child->setBackgroundColor(0, QColor(Qt::gray));
            child->setBackgroundColor(1, QColor(Qt::gray));
            isDescendantA_Target |= true;
        }
    }

    return isDescendantA_Target;
}

void MainWindow::startSimulation() {
    ui_->startSim->hide();
    ui_->pauseSim->show();
    ui_->spinTick->setEnabled(false);
    timer_->start(120);
}

void MainWindow::pauseSimulation() {
    if (!ui_->pauseSim->isHidden()) {
        ui_->startSim->show();
        ui_->pauseSim->hide();
        ui_->spinTick->setEnabled(true);
    }
    timer_->stop();
}

void MainWindow::jumpToSimulation(int tick) {
    temporalIndex_->jumpToTick(tick);
    requestUpdateDynamicData();
}

void MainWindow::jumpToNextTick() {
    unsigned int tick = temporalIndex_->jumpToNextTick();
    if (tick) {
        ui_->sliderTick->setValue(tick);
        ui_->spinTick->setValue(tick);
        requestUpdateDynamicData();
    } else {
        timer_->stop();
    }
}

void MainWindow::requestUpdateDynamicData() {
    requestUpdateMicroData();
}

void MainWindow::requestUpdateMicroData() {
    viewController_->addTask(iSimGUI::UPDATE_MICRO_DATA);
}

void MainWindow::requestUpdateAgents() {
    viewController_->addTask(iSimGUI::UPDATE_AGENTS);
}
