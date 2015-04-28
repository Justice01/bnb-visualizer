#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    PaintWidget *p=new PaintWidget();
    QPen pen= QPen(Qt::red);

    p->setPainterParams(&pen,NULL,PaintWidget::Line);
    ui->verticalLayout_3->addWidget(p);
    p->show();

}

HelpDialog::~HelpDialog()
{
    delete ui;
}
