#include "plotlayout.h"

PlotLayout::PlotLayout(int procNum, int curveLength):QHBoxLayout()
{
    plots = new QList<QwtPlot*>();
    curves = new QList<QwtPlotCurve*>();
    QwtPlot *plot;
    QwtPlotCurve *curve;
    for (int i=0;i<procNum;i++)
    {
        plot=new QwtPlot("Process "+QString::number(i));
        plot->setAxisScale(QwtPlot::xBottom,0 - curveLength,0);
        plot->setAxisScale(QwtPlot::yLeft,0,1.2,1);
        plot->setAxisAutoScale(QwtPlot::xBottom,false);
        plot->setMaximumWidth(200);
        plot->setMaximumHeight(200);
        plots->append(plot);
        curve= new QwtPlotCurve();
        curve->setPen( Qt::blue,4),
        curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve->attach(plot);
        curves->append(curve);
        this->addWidget(plot);
    }
}

PlotLayout::~PlotLayout()
{
    for (int i=0;i<plots->count();i++)
    {
        delete curves->at(i);
        this->removeWidget(plots->at(i));
        delete (plots->at(i));
    }
    curves->clear();
    plots->clear();
    delete curves;
    delete (plots);
}

