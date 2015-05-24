#include "tableprocview.h"
TableProcView::TableProcView(int procNum, int procSize):QGraphicsView()
{
    setDragMode(ScrollHandDrag);
    rects= new QVector<QGraphicsRectItem*>(procNum);
    scene= new QGraphicsScene();
    this->setScene(scene);
    int procLine= std::sqrt((double)procNum);
    for(int i=0;i<procNum;i++)
    {
        (*rects)[i] = new QGraphicsRectItem();
        (*rects)[i]= new QGraphicsRectItem();
        (*rects)[i]->setPen(QPen(Qt::black));
        (*rects)[i]->setBrush(QBrush(Qt::gray));
        (*rects)[i]->setRect((i%procLine+1)*(procSize+2),(i/procLine+1)*(procSize+2), procSize, procSize);
        (*rects)[i]->setToolTip(QString::number(i));
        scene->addItem((*rects)[i]);
    }
}

void TableProcView::wheelEvent(QWheelEvent *event)
{
    double numDegrees= event->delta()/8.0;
    double numSteps=numDegrees/15.0;
    double factor = std::pow(1.125,numSteps);
    scale(factor,factor);
}

TableProcView::~TableProcView()
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

