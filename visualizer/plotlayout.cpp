#include "plotlayout.h"

PlotLayout::PlotLayout(int procNum, int curveLength, int plotMaxSize, int screenWidth):QGridLayout()
{
    plots = new QVector<QwtPlot*>(procNum);
    activityCurves = new QVector<QwtPlotCurve*>(procNum);
    sendingCurves = new QVector<QwtPlotCurve*>(procNum);
    receivingCurves = new QVector<QwtPlotCurve*>(procNum);
    int numProcInLine = screenWidth/plotMaxSize - 1;
    for (int i=0;i<procNum;i++)
    {
        (*plots)[i]=new QwtPlot("Process "+QString::number(i));
        (*plots)[i]->setAxisScale(QwtPlot::xBottom,-curveLength,0);
        (*plots)[i]->setAxisScale(QwtPlot::yLeft,0,1.2,1);
        (*plots)[i]->setAxisAutoScale(QwtPlot::xBottom,false);
        (*plots)[i]->setMaximumWidth(plotMaxSize);
        (*plots)[i]->setMaximumHeight(plotMaxSize);
        (*plots)[i]->setMinimumHeight(plotMaxSize/1.5);
        (*activityCurves)[i] = new QwtPlotCurve();
        (*activityCurves)[i]->setPen( Qt::blue);
        //activityCurve->setBrush(Qt::blue);
        (*activityCurves)[i]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        (*activityCurves)[i]->attach((*plots)[i]);
        (*sendingCurves)[i] = new QwtPlotCurve();
        (*sendingCurves)[i]->setPen( Qt::green);
        //sendingCurve->setBrush(Qt::green);
        (*sendingCurves)[i]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        (*sendingCurves)[i]->attach((*plots)[i]);
        (*receivingCurves)[i] = new QwtPlotCurve();
        (*receivingCurves)[i]->setPen( Qt::red);
        //receivingCurve->setBrush(Qt::red);
        (*receivingCurves)[i]->setRenderHint( QwtPlotItem::RenderAntialiased, true );
        (*receivingCurves)[i]->attach((*plots)[i]);
        this->addWidget((*plots)[i],i/numProcInLine,i%numProcInLine);
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


