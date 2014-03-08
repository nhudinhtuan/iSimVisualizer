#include "mesoscopicdataplotview.h"

MesoscopicDataPlotView::MesoscopicDataPlotView(QWidget *parent, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager, unsigned long segmentId)
    : QWidget(parent), preferenceManager_(preferenceManager), temporalIndex_(temporalIndex), segmentId_(segmentId){
    initUI();
}

void MesoscopicDataPlotView::initUI() {
    // density
    densityPlot_ = new QCustomPlot();
    densityPlot_->setInteractions(QCustomPlot::iRangeDrag|QCustomPlot::iRangeZoom);
    densityPlot_->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    densityPlot_->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    densityPlot_->legend->setVisible(true);

    densityPlot_->addGraph();
    densityPlot_->graph(0)->setName("Mesoscopic Density");
    densityPlot_->xAxis->setLabel("time slice");
    densityPlot_->yAxis->setLabel("cars/km");
    densityPlot_->yAxis->setRange(0, preferenceManager_->getMesoscopicDensityPlotScale());
    densityPlot_->replot();

    densityTracer_ = new QCPItemTracer(densityPlot_);
    densityTracer_->setStyle(QCPItemTracer::tsCrosshair);
    densityTracer_->setGraph(densityPlot_->graph(0));
    densityTracer_->setVisible(true);

    // flow
    flowPlot_ = new QCustomPlot();
    flowPlot_->setInteractions(QCustomPlot::iRangeDrag|QCustomPlot::iRangeZoom);
    flowPlot_->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    flowPlot_->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    flowPlot_->legend->setVisible(true);

    flowPlot_->addGraph();
    flowPlot_->graph(0)->setName("Mesoscopic Flow");
    flowPlot_->xAxis->setLabel("time slice");
    flowPlot_->yAxis->setLabel("cars/sec-meter");
    flowPlot_->yAxis->setRange(0, preferenceManager_->getMesoscopicFlowPlotScale());

    flowPlot_->replot();

    flowTracer_ = new QCPItemTracer(flowPlot_);
    flowTracer_->setStyle(QCPItemTracer::tsCrosshair);
    flowTracer_->setGraph(flowPlot_->graph(0));
    flowTracer_->setVisible(true);

    // speed
    speedPlot_ = new QCustomPlot();
    speedPlot_->setInteractions(QCustomPlot::iRangeDrag|QCustomPlot::iRangeZoom);
    speedPlot_->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    speedPlot_->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    speedPlot_->legend->setVisible(true);

    speedPlot_->addGraph();
    speedPlot_->graph(0)->setName("Mesoscopic Speed");
    speedPlot_->xAxis->setLabel("time slice");
    speedPlot_->yAxis->setLabel("meters/sec");
    speedPlot_->yAxis->setRange(0, preferenceManager_->getMesoscopicSpeedPlotScale());

    speedTracer_ = new QCPItemTracer(speedPlot_);
    speedTracer_->setStyle(QCPItemTracer::tsCrosshair);
    speedTracer_->setGraph(speedPlot_->graph(0));
    speedTracer_->setVisible(true);

    densityPlot_->xAxis->setRange(0, 100);
    flowPlot_->xAxis->setRange(0, 100);
    speedPlot_->xAxis->setRange(0, 100);

    // layout
    QLabel *title = new QLabel();
    title->setText("MESOSCOPIC PLOT VIEW");
    title->setMaximumHeight(10);
    closeBut_ = new QPushButton();
    closeBut_->setText("Close");
    closeBut_->setMaximumWidth(100);
    QHBoxLayout *titleBar = new QHBoxLayout();
    titleBar->addWidget(title);
    titleBar->addWidget(closeBut_);
    QVBoxLayout *plotsContainer = new QVBoxLayout();
    plotsContainer->addLayout(titleBar);
    plotsContainer->addWidget(densityPlot_);
    plotsContainer->addWidget(flowPlot_);
    plotsContainer->addWidget(speedPlot_);
    this->setLayout(plotsContainer);

    connect(closeBut_, SIGNAL(clicked()), this, SIGNAL(announceClose()));
}

void MesoscopicDataPlotView::plotData() {
    if (segmentId_ == 0) return;

    Mesoscopic *data = temporalIndex_->getMesoscopic(segmentId_);
    unsigned int tick = temporalIndex_->getCurrentTick();
    if (data) {
        densityData_.push_back(data->getDensity());
        flowData_.push_back(data->getFlow());
        speedData_.push_back(data->getSpeed());
    } else {
        densityData_.push_back(0);
        flowData_.push_back(0);
        speedData_.push_back(0);
    }

    timeData_.push_back(tick);

    densityPlot_->graph(0)->setData(timeData_, densityData_);
    flowPlot_->graph(0)->setData(timeData_, flowData_);
    speedPlot_->graph(0)->setData(timeData_, speedData_);
    densityTracer_->setGraphKey(tick);
    flowTracer_->setGraphKey(tick);
    speedTracer_->setGraphKey(tick);
    densityPlot_->replot();
    flowPlot_->replot();
    speedPlot_->replot();
}
