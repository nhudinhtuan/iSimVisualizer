#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProgressBar>
#include <QGraphicsScene>
#include <QTime>
#include <QInputDialog>
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QVariant>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include "dialog/openfiledialog.h"
#include "dialog/preferencedialog.h"
#include "dialog/dbviewdialog.h"
#include "io/dbmanager.h"
#include "io/filereader.h"
#include "io/dbloader.h"
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "preferencemanager.h"
#include "viewcontroller.h"
#include "graphicsview/mapgraphicsview.h"
#include "plotview/mesoscopicdataplotview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void about();
    void open();
    void addLog(QString log);
    void saveLog();
    void alertFileError(QString error);
    void hideLeftStackedWidget();
    void openLogPage();
    void openGeospatialElementsPage();
    void openDynamicPage();
    void findLocation();
    void updateProgressBar(int);
    void loadGeospatial();
    void showSimulationGUI();
    void enableSimulationGUI();
    void updateUpperTickValue(unsigned int);
    void finishLoadingGeospatial();
    void loadFileCompleted();
    void fileReaderTerminated();
    void updatePointerTracker(QPoint);
    void openPreferencesDialog();

    void createGUniNode(UniNode *uniNode);
    void createGMultiNode(MultiNode *multiNode);
    void createGBusStop(BusStop *busStop);
    void createGSegment(RoadSegment *segment);
    void createGLane(Lane *lane);
    void createGLaneConnector(LaneConnector *laneConnector);
    void createGCrossing(Crossing *crossing);
    void createGTrafficSignal(TrafficSignal *crossing);
    void createGIncident(Incident *incident);
    void createLinkTreeItem(Link *link);

    void updateMapView();
    void focusOnGraphicsOfTreeItem(QTreeWidgetItem *item, int column);
    void updateSelectedItems();
    void searchGeo(QString key);
    void setGeoTreeItemsToDefaultState(QTreeWidgetItem* item);
    void filterGeoTreeItems(QList<QTreeWidgetItem*>&);
    bool filterGeoTreeItemsRecursive(QList<QTreeWidgetItem*>&, QTreeWidgetItem*);

    void startSimulation();
    void pauseSimulation();
    void jumpToNextTick();
    void jumpToSimulation(int);

    void requestUpdateDynamicData();
    void requestUpdateMicroData();
    void requestUpdateAgents();
    void updateGAgents(AgentList* agents);

    void updateDBConfig();
    void updateOverlayTitle();
    void updateMesosOverlay();

    void createChart(unsigned long segmentId);
    void closeChart();
    void openDBViewDialog();
    void requestResetWorkspace();
    void loadRecordFromDB(int fileId);

private:
    void resetWorkspace();
    void initData();
    void initUi();
    void connectSignalAction();
    void resetUi();
    void showNodeProperty(Node *data);
    void showLinkProperty(Link *data);
    void showSegmentProperty(RoadSegment *data);
    void showLaneProperty(Lane *data);
    void showCrossingProperty(Crossing *data);
    void showBusStopProperty(BusStop *data);

    G_Agent* gAgentFactory(Agent*);
    void showGAgentProperty(G_Agent *data);


    QTreeWidgetItem* getTreeItemFromGraphics(QGraphicsItem *gItem);

    Ui::MainWindow *ui_;
    FileReader *fileReader_;
    ViewController *viewController_;
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;
    PreferenceManager *preferenceManager_;
    DBManager *dbManager_;
    DBLoader *dbLoader_;
    DBViewDialog *loadDBDialog_;

    // components for status bar
    QProgressBar *progressBar_;
    QLabel *pointerTracker_;

    // tree widget items
    QTreeWidgetItem *uninodeTreeItems_;
    QTreeWidgetItem *multinodeTreeItems_;
    QTreeWidgetItem *linkLabelTreeItems_;
    QHash<unsigned long, QTreeWidgetItem*> linkTreeItems_;
    QHash<unsigned long, QTreeWidgetItem*> segmentItems_;
    QTreeWidgetItem *busStopTreeItems_;
    QTreeWidgetItem *crossingTreeItems_;

    //map view
    QGraphicsScene *scene_;
    MapGraphicsView *mapView_;

    // plot view
    MesoscopicDataPlotView *plotView_;

    //timer
    QTimer *timer_;

    // G_Agent
    QHash<unsigned long, G_Agent*> gAgents_;

    QTime profile_;
};

#endif // MAINWINDOW_H
