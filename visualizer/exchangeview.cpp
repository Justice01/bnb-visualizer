#include "exchangeview.h"
ExchangeView::ExchangeView(int procNum, int procSize):QGraphicsView()
{
    setDragMode(ScrollHandDrag);
    senders= new QVector<QGraphicsRectItem*>(procNum);
    receivers= new QVector<QGraphicsRectItem*>(procNum);
    scene= new QGraphicsScene();
    this->setScene(scene);
    for(int i=0;i<procNum;i++)
    {
        (*senders)[i]= new QGraphicsRectItem();
        (*senders)[i]->setPen(QPen(Qt::black));
        (*senders)[i]->setBrush(QBrush(Qt::gray));
        (*senders)[i]->setRect((i+1)*(procSize+2),0,procSize,procSize);
        (*senders)[i]->setToolTip(QString::number(i));
        //senders->append(sndr);
        scene->addItem((*senders)[i]);
        (*receivers)[i] = new QGraphicsRectItem();
        (*receivers)[i]->setPen(QPen(Qt::black));
        (*receivers)[i]->setBrush(QBrush(Qt::gray));
        (*receivers)[i]->setRect(0,-(i+1)*(procSize+2),procSize,procSize);
        (*receivers)[i]->setToolTip(QString::number(i));
        //receivers->append(rcvr);
        scene->addItem((*receivers)[i]);
    }
}

void ExchangeView::wheelEvent(QWheelEvent *event)
{
    double numDegrees= event->delta()/8.0;
    double numSteps=numDegrees/15.0;
    double factor = std::pow(1.125,numSteps);
    scale(factor,factor);
}

ExchangeView::~ExchangeView()
{
    for (int i=0;i<senders->count();i++)
    {
        delete senders->at(i);
        delete receivers->at(i);
    }
    scene->clear();
    senders->clear();
    receivers->clear();
    delete senders;
    delete receivers;
    delete scene;
}

