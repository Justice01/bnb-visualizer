#include "exchanger.h"

exchanger::exchanger(int from, int to, QGraphicsScene * scene, int start, int end)
{
    _from=from;
    _to=to;
    _scene=scene;
    _start=start;
    _end=end;
    _lineFrom=NULL;
    _lineTo=NULL;
}

void exchanger::addScene(QGraphicsScene * scene)
{
    _scene=scene;
}

void exchanger::drawLines(int time, QVector<QGraphicsRectItem *> *senders, QVector<QGraphicsRectItem *> *receivers)
{

    if(time>=_start && time<=_end)
    {
        if(_lineFrom==NULL)
        {
            _lineFrom = new QGraphicsLineItem((*senders)[_from]->rect().x()+(*senders)[_from]->rect().width()/2,(*senders)[_from]->rect().y(),(*senders)[_from]->rect().x()+(*senders)[_from]->rect().width()/2,(*receivers)[_to]->rect().y()+(*receivers)[_to]->rect().height()/2);
            _lineFrom->setPen(QPen(Qt::green));
            _scene->addItem(_lineFrom);
        }
        if(_lineTo==NULL && time==_end)
        {
            _lineTo = new QGraphicsLineItem((*senders)[_from]->rect().x()+(*senders)[_from]->rect().width()/2,(*receivers)[_to]->rect().y()+(*receivers)[_to]->rect().height()/2,(*receivers)[_to]->rect().x()+(*receivers)[_to]->rect().width(),(*receivers)[_to]->rect().y()+(*receivers)[_to]->rect().height()/2);
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

int exchanger::getFrom()
{
    return _from;
}

int exchanger::getTo()
{
    return _to;
}

void exchanger::setFrom(int from)
{
    _from=from;
}

void exchanger::setTo(int to)
{
    _to=to;
}

int exchanger::getStart()
{
    return _start;
}

int exchanger::getEnd()
{
    return _end;
}

void exchanger::setStart(int start)
{
    _start=start;
}

void exchanger::setEnd(int end)
{
    _end=end;
}

exchanger::~exchanger()
{
    delete _lineFrom;
    delete _lineTo;
}

