#include "plotlayout.h"

PlotLayout::PlotLayout(int procNum, int curveLength, int plotMaxSize, int screenWidth):QGridLayout()
{
    plots = new QList<QwtPlot*>();
    curves = new QList<QwtPlotCurve*>();
    QwtPlot *plot;
    QwtPlotCurve *curve;
    int numProcInLine = screenWidth/plotMaxSize - 1;
    for (int i=0;i<procNum;i++)
    {
        plot=new QwtPlot("Process "+QString::number(i));
        plot->setAxisScale(QwtPlot::xBottom,-curveLength,0);
        plot->setAxisScale(QwtPlot::yLeft,0,1.2,1);
        plot->setAxisAutoScale(QwtPlot::xBottom,false);
        plot->setMaximumWidth(plotMaxSize);
        plot->setMaximumHeight(plotMaxSize);
        plot->setMinimumHeight(plotMaxSize/1.5);
        plots->append(plot);
        curve = new QwtPlotCurve();
        curve->setPen( Qt::blue);
        curve->setBrush(Qt::blue);
        curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        curve->attach(plot);
        curves->append(curve);
        this->addWidget(plot,i/numProcInLine,i%numProcInLine);
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
    delete plots;
}


