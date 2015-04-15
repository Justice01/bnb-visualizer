#include "exchangeview.h"

ExchangeView::ExchangeView(int procNum, int procSize):QGraphicsView()
{
    setDragMode(ScrollHandDrag);
    senders= new QList<QGraphicsRectItem*>();
    receivers= new QList<QGraphicsRectItem*>();
    QGraphicsRectItem *sndr;
    QGraphicsRectItem *rcvr;
    scene= new QGraphicsScene();
    this->setScene(scene);
    for(int i=0;i<procNum;i++)
    {
        sndr = new QGraphicsRectItem();
        sndr= new QGraphicsRectItem();
        sndr->setPen(QPen(Qt::black));
        sndr->setBrush(QBrush(Qt::gray));
        sndr->setRect((i+1)*(procSize+2),0,procSize,procSize);
        sndr->setToolTip(QString::number(i));
        senders->append(sndr);
        scene->addItem(sndr);
        rcvr = new QGraphicsRectItem();
        rcvr= new QGraphicsRectItem();
        rcvr->setPen(QPen(Qt::black));
        rcvr->setBrush(QBrush(Qt::gray));
        rcvr->setRect(0,-(i+1)*(procSize+2),procSize,procSize);
        rcvr->setToolTip(QString::number(i));
        receivers->append(rcvr);
        scene->addItem(rcvr);
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

