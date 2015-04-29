#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <qpainter.h>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    enum  PaintType
    {
        Line,
        Rect,
        None
    };
    explicit PaintWidget(QWidget *parent = 0);
    PaintWidget(QPen *pen, QBrush *brush, PaintType type, QWidget *parent = 0);
    void setPainterParams(QPen *pen, QBrush *brush, PaintType type);

    ~PaintWidget();

protected:
    void paintEvent(QPaintEvent *event);
private:
    QPen *_pen;
    QBrush *_brush;
    PaintType _type;
signals:

public slots:
};
#endif // PAINTWIDGET_H
