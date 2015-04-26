#ifndef EXCHANGER_H
#define EXCHANGER_H
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>

class exchanger
{
private:
    int _from;
    int _to;
    QGraphicsScene * _scene;
    int _start;
    int _end;
    QGraphicsLineItem * _lineFrom;
    QGraphicsLineItem * _lineTo;
public:
    exchanger(int from, int to, QGraphicsScene * scene, int start, int end);
    void addScene(QGraphicsScene * scene);
    void drawLines(int time, QVector<QGraphicsRectItem*> *senders,QVector<QGraphicsRectItem*> *receivers);
    int getFrom();
    int getTo();
    void setFrom(int from);
    void setTo(int to);
    int getStart();
    int getEnd();
    void setStart(int start);
    void setEnd(int end);
    virtual ~exchanger();
};

#endif // EXCHANGER_H
