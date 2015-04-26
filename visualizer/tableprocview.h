#ifndef TABLEPROCVIEW_H
#define TABLEPROCVIEW_H
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <cmath>
#include <qevent.h>

class TableProcView : public QGraphicsView
{
public:
    TableProcView(int procNum, int procSize);
    QList<QGraphicsRectItem*> *rects;
    QGraphicsScene * scene;
    virtual ~TableProcView();
protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // TABLEPROCVIEW_H
