#include "exchangescene.h"

ExchangeScene::ExchangeScene(int procNum, int procSize, int screenWidth):QGraphicsScene()
{
    senders= new QList<QGraphicsRectItem*>();
    QGraphicsRectItem *sndr;
    QGraphicsRectItem *rcvr;
    for(int i=0;i<procNum;i++)
    {
        sndr = new QGraphicsRectItem();
        sndr= new QGraphicsRectItem();
        sndr->setPen(QPen(Qt::black));
        sndr->setBrush(QBrush(Qt::black));
        sndr->setRect((i+1)*(procSize+2),0,procSize,procSize);
        senders->append(sndr);
        this->addItem(sndr);
        rcvr = new QGraphicsRectItem();
        rcvr= new QGraphicsRectItem();
        rcvr->setPen(QPen(Qt::black));
        rcvr->setBrush(QBrush(Qt::black));
        rcvr->setRect(0,-(i+1)*(procSize+2),procSize,procSize);
        receivers->append(rcvr);
        this->addItem(rcvr);
    }
}

ExchangeScene::~ExchangeScene()
{

}

