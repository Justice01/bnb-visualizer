#include "exchanger.h"

exchanger::exchanger(int from, int to, QGraphicsScene * scene, int start, int end, QList<exchanger *> *eList)
{
    _from=from;
    _to=to;
    _scene=scene;
    _start=start;
    _end=end;
    _eList=eList;
    _lineFrom=NULL;
    _lineTo=NULL;
}

void exchanger::drawLines(int time, QVector<QGraphicsRectItem *> *senders, QVector<QGraphicsRectItem *> *receivers)
{

    if(time>=_start && time<=_end)
    {
        if(_lineFrom==NULL)
        {
            _lineFrom = new QGraphicsLineItem((*senders)[_from]->x()+(*senders)[_from]->rect().width()/2,(*senders)[_from]->y(),(*senders)[_from]->rect().x()+(*senders)[_from]->rect().width()/2,(*receivers)[_to]->y()+(*receivers)[_to]->rect().height()/2);
            _lineFrom->setPen(QPen(Qt::green));
            _scene->addItem(_lineFrom);
        }
        if(_lineTo==NULL && time==_end)
        {
            _lineTo = new QGraphicsLineItem((*senders)[_from]->x()+(*senders)[_from]->rect().width()/2,(*receivers)[_to]->y()+(*receivers)[_to]->rect().height()/2,(*receivers)[_to]->rect().x()+(*receivers)[_to]->rect().width(),(*receivers)[_to]->y()+(*receivers)[_to]->rect().height()/2);
            _lineTo->setPen(QPen(Qt::blue));
            _scene->addItem(_lineTo);
        }
        else if(_lineTo!=NULL)
        {
            _scene->removeItem(_lineTo);
            delete _lineTo;
            _lineTo=NULL;
        }
    }
    else
    {
        if(_lineFrom!=NULL)
        {
            _scene->removeItem(_lineFrom);
            delete _lineFrom;
            _lineFrom=NULL;
        }
        if(_lineTo!=NULL)
        {
            _scene->removeItem(_lineTo);
            delete _lineTo;
            _lineTo=NULL;
        }
    }
}

exchanger::~exchanger()
{

}

