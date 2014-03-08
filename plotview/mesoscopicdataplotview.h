#ifndef MESOSCOPICDATAPLOTVIEW_H
#define MESOSCOPICDATAPLOTVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "plotview/qcustomplot.h"
#include "preferencemanager.h"
#include "temporal/temporalindex.h"

class MesoscopicDataPlotView: public QWidget
{
    Q_OBJECT

public:
    explicit MesoscopicDataPlotView(QWidget *parent, TemporalIndex *temporalIndex, PreferenceManager *preferenceManager, unsigned long segmentId);
    void initUI();

public slots:
    void plotData();

signals:
    void announceClose();

private:
    PreferenceManager *preferenceManager_;
    TemporalIndex *temporalIndex_;
    unsigned long segmentId_;

    QCustomPlot *densityPlot_;
    QCustomPlot *flowPlot_;
    QCustomPlot *speedPlot_;

    QCPItemTracer *densityTracer_;
    QCPItemTracer *flowTracer_;
    QCPItemTracer *speedTracer_;

    QVector<double> densityData_;
    QVector<double> flowData_;
    QVector<double> speedData_;
    QVector<double> timeData_;

    QPushButton *closeBut_;

};

#endif // MESOSCOPICDATAPLOTVIEW_H
