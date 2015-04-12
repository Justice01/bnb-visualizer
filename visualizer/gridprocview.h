#ifndef GRIDPROCVIEW_H
#define GRIDPROCVIEW_H
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <cmath>
#include <qevent.h>

class GridProcView : public QGraphicsView
{
public:
    GridProcView(int procNum, int procSize, int screenWidth);
    QList<QGraphicsRectItem*> *rects;
    QGraphicsScene * scene;
    virtual ~GridProcView();
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // GRIDPROCVIEW_H
