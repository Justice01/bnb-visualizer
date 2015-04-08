#ifndef EXCHANGESCENE_H
#define EXCHANGESCENE_H
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>

class ExchangeScene : public QGraphicsScene
{
public:
    ExchangeScene(int procNum, int procSize, int screenWidth);
    QList<QGraphicsRectItem*> *senders;
    QList<QGraphicsRectItem*> *receivers;

    virtual ~ExchangeScene();

};

#endif // EXCHANGESCENE_H
