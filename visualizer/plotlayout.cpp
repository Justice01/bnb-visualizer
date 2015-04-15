#include "plotlayout.h"

PlotLayout::PlotLayout(int procNum, int curveLength, int plotMaxSize, int screenWidth):QGridLayout()
{
    plots = new QList<QwtPlot*>();
    activityCurves = new QList<QwtPlotCurve*>();
    sendingCurves = new QList<QwtPlotCurve*>();
    receivingCurves = new QList<QwtPlotCurve*>();
    QwtPlot *plot;
    QwtPlotCurve *activityCurve;
    QwtPlotCurve *sendingCurve;
    QwtPlotCurve *receivingCurve;
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
        activityCurve = new QwtPlotCurve();
        activityCurve->setPen( Qt::blue);
        //activityCurve->setBrush(Qt::blue);
        activityCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        activityCurve->attach(plot);
        activityCurves->append(activityCurve);
        sendingCurve = new QwtPlotCurve();
        sendingCurve->setPen( Qt::green);
        //sendingCurve->setBrush(Qt::green);
        sendingCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        sendingCurve->attach(plot);
        sendingCurves->append(sendingCurve);
        receivingCurve = new QwtPlotCurve();
        receivingCurve->setPen( Qt::red);
        //receivingCurve->setBrush(Qt::red);
        receivingCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        receivingCurve->attach(plot);
        receivingCurves->append(receivingCurve);
        this->addWidget(plot,i/numProcInLine,i%numProcInLine);
    }
}

PlotLayout::~PlotLayout()
{
    for (int i=0;i<plots->count();i++)
    {
        delete activityCurves->at(i);
        delete sendingCurves->at(i);
        delete receivingCurves->at(i);
        this->removeWidget(plots->at(i));
        delete (plots->at(i));
    }
    activityCurves->clear();
    sendingCurves->clear();
    receivingCurves->clear();
    plots->clear();
    delete activityCurves;
    delete sendingCurves;
    delete receivingCurves;
    delete plots;
}


