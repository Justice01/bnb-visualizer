#ifndef EXCHANGER_H
#define EXCHANGER_H
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>

class exchanger
{
private:
    //QGraphicsRectItem * _from;
    //QGraphicsRectItem * _to;
    int _from;
    int _to;
    QGraphicsScene * _scene;
    int _start;
    int _end;
    QGraphicsLineItem *_lineFrom;
    QGraphicsLineItem *_lineTo;
    QList<exchanger*> * _eList;
public:
    exchanger(int from, int to, QGraphicsScene * scene, int start, int end, QList<exchanger*> * eList);
    void drawLines(int time, QVector<QGraphicsRectItem*> *senders,QVector<QGraphicsRectItem*> *receivers);
    virtual ~exchanger();
};

#endif // EXCHANGER_H
