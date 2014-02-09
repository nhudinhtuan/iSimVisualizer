#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProgressBar>
#include <QGraphicsScene>
#include <QTime>
#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>
#include "dialog/openfiledialog.h"
#include "io/filereader.h"
#include "geospatial/geospatialindex.h"
#include "temporal/temporalindex.h"
#include "preferencemanager.h"
#include "viewcontroller.h"
#include "graphicsview/mapgraphicsview.h"

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
    void openBusRoutePage();
    void findLocation();
    void updateProgressBar(int);
    void loadGeospatial();
    void finishLoadFile();
    void updatePointerTracker(QPoint);

    void createGNode(Node *node);
    void createGBusStop(BusStop *busStop);
    void createGSegment(RoadSegment *segment);
    void createGLane(Lane *lane);

private:
    void initData();
    void initUi();
    void connectSignalAction();
    void resetWorkspace();
    void resetUi();

    Ui::MainWindow *ui_;
    FileReader *fileReader_;

    ViewController *viewController_;
    GeospatialIndex *geospatialIndex_;
    TemporalIndex *temporalIndex_;
    PreferenceManager *preferenceManager_;

    // components for status bar
    QProgressBar *progressBar;
    QLabel *pointerTracker;

    //map view
    QGraphicsScene *scene_;
    MapGraphicsView *mapView_;
};

#endif // MAINWINDOW_H
