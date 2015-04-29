#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    _pen=NULL;
    _brush=NULL;
}

PaintWidget::PaintWidget(QPen *pen, QBrush *brush, PaintType type, QWidget *parent) : QWidget(parent)
{
    _pen=pen;
    _brush=brush;
    _type=type;
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (_pen!=NULL) p.setPen(*_pen);
    if (_brush!=NULL)p.setBrush(*_brush);
    switch(_type)
    {
    case Line:
        p.drawLine(this->rect().x(),this->rect().y()+this->rect().height()/2,this->rect().x()+this->rect().width(),this->rect().y()+this->rect().height()/2);
        break;
    case Rect:
        p.drawRect(this->rect().x(),this->rect().y(),this->rect().width(),this->rect().height());
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
    if(_pen!=NULL) delete _pen;
    if(_brush!=NULL) delete _brush;
}

