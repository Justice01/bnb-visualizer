#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(*_pen);
    p.setBrush(*_brush);
    p.drawLine(this->rect().x(),this->rect().y(),this->rect().x()+this->rect().width(),this->rect().y()+this->rect().height());
    switch(_type)
    {
    case Line:
        p.drawLine(this->rect().x(),this->rect().y(),this->rect().x()+this->rect().width(),this->rect().y()+this->rect().height());
        qWarning("Ok!");
        break;
    case Rect:
        break;
    }
}

void PaintWidget::setPainterParams(QPen *pen, QBrush *brush, PaintType type)
{
    _pen=pen;
    _brush=brush;
    _type=type;
}

PaintWidget::~PaintWidget()
{

}

