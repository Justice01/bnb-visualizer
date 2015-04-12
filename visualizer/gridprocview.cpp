#include "gridprocview.h"

GridProcView::GridProcView(int procNum, int procSize, int screenWidth):QGraphicsView()
{
    setDragMode(ScrollHandDrag);
    rects= new QList<QGraphicsRectItem*>();
    QGraphicsRectItem *rect;
    scene= new QGraphicsScene();
    this->setScene(scene);
    int procLine= std::sqrt(procNum);
    for(int i=0;i<procNum;i++)
    {
        rect = new QGraphicsRectItem();
        rect= new QGraphicsRectItem();
        rect->setPen(QPen(Qt::black));
        rect->setBrush(QBrush(Qt::gray));
        rect->setRect((i%procLine+1)*(procSize+2),(i/procLine+1)*(procSize+2), procSize, procSize);
        rect->setToolTip(QString::number(i));
        rects->append(rect);
        scene->addItem(rect);
    }
}

void GridProcView::wheelEvent(QWheelEvent *event)
{
    double numDegrees= event->delta()/8.0;
    double numSteps=numDegrees/15.0;
    double factor = std::pow(1.125,numSteps);
    scale(factor,factor);
}

GridProcView::~GridProcView()
{
    for (int i=0;i<rects->count();i++)
    {
        delete rects->at(i);
    }
    scene->clear();
    rects->clear();
    delete rects;
    delete scene;
}

