#include "helpdialog.h"
#include "ui_helpdialog.h"
HelpDialog::HelpDialog(TabType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    fillHelp();
    switch(type)
    {
    case PLOT:
        ui->tabWidget->setCurrentWidget(ui->plotTab);
        break;
    case TABLE:
        ui->tabWidget->setCurrentWidget(ui->tableTab);
        break;
    case EXCHANGE:
        ui->tabWidget->setCurrentWidget(ui->exchangeTab);
        break;
    }
}

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    fillHelp();
}

void HelpDialog::fillHelp()
{
    lbls= new QList<QLabel*>();
    wdgts= new  QList<PaintWidget*>();
    PaintWidget *p;
    QLabel *l;
    /*
     * plot tab
     */
    //solve
    p=new PaintWidget(new QPen(Qt::blue),NULL, PaintWidget::Line);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE*2);
    ui->solvePlotLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("solving, e.a. processors behavior between action \"solve\" and event \"done\"");
    ui->solvePlotLayout->addWidget(l);
    //send
    p=new PaintWidget(new QPen(Qt::green),NULL, PaintWidget::Line);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE*2);
    ui->sendPlotLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("sending, e.a. processors behavior between action \"send\" (command, records, sub, sub and records) and event \"sent\"");
    ui->sendPlotLayout->addWidget(l);
    //receive
    p=new PaintWidget(new QPen(Qt::red),NULL, PaintWidget::Line);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE*2);
    ui->recvPlotLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("receiving, e.a. processors behavior between action \"reciive\" and event \"arrived\" (data, command)");
    ui->recvPlotLayout->addWidget(l);
    /*
     * table tab
     */
    //solve
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::blue), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->solveTableLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("solving, e.a. processors behavior between action \"solve\" and event \"done\"");
    ui->solveTableLayout->addWidget(l);
    //send
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::green), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->sendTableLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("sending, e.a. processors behavior between action \"send\" (command, records, sub, sub and records) and event \"sent\"");
    ui->sendTableLayout->addWidget(l);
    //receive
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::red), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->recvTableLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("receiving, e.a. processors behavior between action \"reciive\" and event \"arrived\" (data, command)");
    ui->recvTableLayout->addWidget(l);
    //none
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::gray), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->noneTableLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("others, e.a. processors behavior while it is not solving or sending or receiving");
    ui->noneTableLayout->addWidget(l);
    /*
     * exchange tab
     */
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("Horisontal rectangles are senders. Vertical rectangles are receivers.");
    ui->mainExchangeLayout->addWidget(l);
    //solve
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::blue), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->solveExchangeLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("solving, e.a. processors behavior between action \"solve\" and event \"done\"");
    ui->solveExchangeLayout->addWidget(l);
    //send
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::green), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->sendExchangeLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("sending, e.a. processors behavior between action \"send\" (command, records, sub, sub and records) and event \"sent\"");
    ui->sendExchangeLayout->addWidget(l);
    //receive
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::red), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->recvExchangeLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("receiving, e.a. processors behavior between action \"reciive\" and event \"arrived\" (data, command)");
    ui->recvExchangeLayout->addWidget(l);
    //none
    p=new PaintWidget(new QPen(Qt::black),new QBrush(Qt::gray), PaintWidget::Rect);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE);
    ui->noneExchangeLayout->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("others, e.a. processors behavior while it is not solving or sending or receiving");
    ui->noneExchangeLayout->addWidget(l);
    //sent
    p=new PaintWidget(new QPen(Qt::green),NULL, PaintWidget::Line);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE*2);
    ui->sendExchangeLayout_2->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("this indicator becomes visible when sender process sent some information to receiver process and becomes invisible when information is received by receiver process");
    ui->sendExchangeLayout_2->addWidget(l);
    //arrived
    p=new PaintWidget(new QPen(Qt::blue),NULL, PaintWidget::Line);
    wdgts->append(p);
    p->setMaximumHeight(RECT_SIZE);
    p->setMaximumWidth(RECT_SIZE*2);
    ui->recvExchangeLayout_2->addWidget(p);
    l= new QLabel();
    lbls->append(l);
    l->setWordWrap(true);
    l->setText("this indicator is shown while sender`s information is arriving to receiver");
    ui->recvExchangeLayout_2->addWidget(l);
}

HelpDialog::~HelpDialog()
{
    delete ui;
    QListIterator<QLabel*> itl(*lbls);
    while(itl.hasNext()) delete itl.next();
    QListIterator<PaintWidget*> itw(*wdgts);
    while(itw.hasNext()) delete itw.next();
}
