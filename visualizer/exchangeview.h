#ifndef EXCHANGEVIEW_H
#define EXCHANGEVIEW_H
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <cmath>
#include <qevent.h>

class ExchangeView : public QGraphicsView
{
public:
    ExchangeView(int procNum, int procSize, int screenWidth);
    QList<QGraphicsRectItem*> *senders;
    QList<QGraphicsRectItem*> *receivers;
    QGraphicsScene * scene;
    virtual ~ExchangeView();
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // EXCHANGEVIEW_H
