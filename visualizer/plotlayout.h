#ifndef PLOTLAYOUT_H
#define PLOTLAYOUT_H
#include <qboxlayout.h>
#include<qwt_plot.h>
#include<qwt_plot_curve.h>
class PlotLayout : public QHBoxLayout
{

public:


    explicit PlotLayout(int procNum, int curveLength);

    QList<QwtPlot*> *plots;
    QList<QwtPlotCurve*> *curves;

    virtual ~PlotLayout();
};

#endif // PLOTLAYOUT_H
