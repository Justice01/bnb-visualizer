#ifndef PLOTLAYOUT_H
#define PLOTLAYOUT_H
#include <qboxlayout.h>
#include<qwt_plot.h>
#include<qwt_plot_curve.h>
#include<qevent.h>
class PlotLayout : public QGridLayout
{
public:
    PlotLayout(int procNum, int curveLength, int plotMaxSize, int screenWidth);
    QVector<QwtPlot*> *plots;
    QVector<QwtPlotCurve*> *activityCurves;
    QVector<QwtPlotCurve*> *receivingCurves;
    QVector<QwtPlotCurve*> *sendingCurves;
    virtual ~PlotLayout();
};

#endif // PLOTLAYOUT_H
