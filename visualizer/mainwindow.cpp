#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plots=NULL;
    activityCurves=NULL;
    sendingCurves=NULL;
    receivingCurves=NULL;
    rects=NULL;
    maxTime=0;
    procNum=0;
    connect(ui->actionLoad_trace,SIGNAL(triggered()),this,SLOT(loadTrace()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ComputeButton_clicked()
{

    //preparing input data
    if(ui->ProcNumEdit->text().toInt()==0)
    {
        QMessageBox::information(this,"","Wrong process number!");
        return;
    }
    if(ui->StepsEdit->text().toInt()==0)
    {
        QMessageBox::information(this,"","Wrong step!");
        return;
    }
    //reading bnb-simulators data (trace)
    QByteArray ar;
    QProcess *p = new QProcess();
    p->setReadChannelMode(QProcess::MergedChannels);
    p->start("./bnbtest");
    p->write(QString(ui->ProcNumEdit->text()+" "+ui->StepsEdit->text()).toLocal8Bit());
    p->closeWriteChannel();
    while(p->waitForStarted(-1))
    {

        while(p->waitForReadyRead(-1))
        {
            ar += p->readAllStandardOutput();
        }
    }
    p->waitForFinished();
    QStringList trace=QString::fromLocal8Bit(ar).split('\n');
    if(trace.last().isEmpty())trace.removeLast();

    prepareVisualization(trace,ui->ProcNumEdit->text().toInt());
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if(ui->horizontalSlider->value() < ui->horizontalSlider->maximum())ui->horizontalSlider->setValue(ui->horizontalSlider->value()+ui->DTSlider->value());
    if (ui->horizontalSlider->value()==ui->horizontalSlider->maximum()) killTimer(timerID);
}

void MainWindow::on_PlayButton_clicked()
{
    killTimer(timerID);
    timerID=startTimer(100);
}

void MainWindow::on_StopButton_clicked()
{
    killTimer(timerID);
    ui->horizontalSlider->setValue(0);
}

void MainWindow::on_PauseButton_clicked()
{
    killTimer(timerID);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    updatePlots(value);
}

void MainWindow::updatePlots(int value)
{
    if(ui->tabWidget->currentWidget()==ui->PlotTab)
    {

        for(int j=0;j < plots->length();j++)
        {
            plots->at(j)->setAxisScale( QwtPlot::xBottom, value - CURVE_MIN_LENGTH - ui->XScaleSlider->value(), value);
            int x = value;
            for(int i=CURVE_MIN_LENGTH + ui->XScaleSlider->value()-1;i>=0;i--)
            {
                if(x>0)
                {
                    time[i] = x;
                    subproc.activity[i] = procs[j].activity[x];
                    subproc.sending[i] = procs[j].sending[x];
                    subproc.receiving[i] = procs[j].receiving[x];
                }
                else
                {
                    time[i]=0.0;
                    subproc.activity[i] = 0.0;
                    subproc.sending[i] = 0.0;
                    subproc.receiving[i] = 0.0;
                }
                x--;
            }
            activityCurves->at(j)->setSamples(time, subproc.activity);
            sendingCurves->at(j)->setSamples(time, subproc.sending);
            receivingCurves->at(j)->setSamples(time, subproc.receiving);
            plots->at(j)->replot();
        }
    }
    if(ui->tabWidget->currentWidget()==ui->GridProcTab)
    {
        for (int i=0;i<rects->length();i++)
        {
            if(procs[i].activity[value]==1)
            {
                rects->at(i)->setBrush(Qt::blue);
            }
            else if(procs[i].sending[value]==1)
            {
                rects->at(i)->setBrush(Qt::green);
            }
            else if(procs[i].receiving[value]==1)
            {
                rects->at(i)->setBrush(Qt::red);
            }
            else
            {
                rects->at(i)->setBrush(Qt::gray);
            }
        }
    }
    if(ui->tabWidget->currentWidget()==ui->ExchangeTab)
    {
        //future code
    }
}

void MainWindow::on_XScaleSlider_valueChanged(int value)
{
    time.resize(CURVE_MIN_LENGTH+value);
    subproc.activity.resize(CURVE_MIN_LENGTH+value);
    subproc.sending.resize(CURVE_MIN_LENGTH+value);
    subproc.receiving.resize(CURVE_MIN_LENGTH+value);
    updatePlots(ui->horizontalSlider->value());
}

void MainWindow::loadTrace()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Open file"),
                                QDir::currentPath(),
                                tr("Text (*.txt)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this,"","File could not be read!");
        return;
    }
    QTextStream reader(&file);
    QStringList trace;
    while(!reader.atEnd())
    {
        trace.append(reader.readLine());
    }
    file.close();
    prepareVisualization(trace);
}

void MainWindow::prepareVisualization(QStringList&trace, int procNum)
{
    //stop timer if started
    killTimer(timerID);
    /*
     * parcing trace
     */
    QStringList traceLine;
    bool procNumSet=true;
    if (procNum==0)procNumSet=false;
    //if second parametr 0 determine procNum from trace
    if (!procNumSet)
    {
        for(int i=0;i<trace.length();i++)
        {
            traceLine=trace[i].split(' ');
            if(traceLine.length()>2)
            {
                if(traceLine.at(1).toInt()>=procNum && traceLine.at(2).toInt()==6) procNum=traceLine.at(1).toInt()+1;
                else if(traceLine.at(1).toInt()<procNum || traceLine.at(2).toInt()!=6) break;
            }
        }
    }
    this->procNum=procNum;
    procs.resize(procNum);
    procs.squeeze();
    int currentProc=0;
    int steps=0;
    maxTime = trace.last().split(' ').first().toInt();
    int solves[procNum];
    int dones[procNum];
    int sends[procNum];
    int sents[procNum];
    int recvs[procNum];
    int arrives[procNum];

    for (int i=0; i<procNum;i++)
    {
        solves[i]=0;
        dones[i]=0;
        sends[i]=0;
        sents[i]=0;
        recvs[i]=0;
        arrives[i]=0;
        procs[i].activity.resize(maxTime+1);
        procs[i].activity.squeeze();
        procs[i].receiving.resize(maxTime+1);
        procs[i].receiving.squeeze();
        procs[i].sending.resize(maxTime+1);
        procs[i].sending.squeeze();
        for (int j=0;j<=maxTime;j++)
        {
            procs[i].activity[j]=0;
            procs[i].receiving[j]=0;
            procs[i].sending[j]=0;
        }
    }
    //reading trace
    for (int j=0; j<trace.length();j++)
    {
        traceLine = trace[j].split(' ');
        //reading events
        if(traceLine.length()>7)
        {
            if(traceLine.at(7).toInt()==BNBScheduler::Events::DONE)
            {
                currentProc=traceLine.at(1).toInt();
                dones[currentProc]=traceLine.at(0).toInt();
                for (int i=solves[currentProc];i<dones[currentProc];i++) procs[currentProc].activity[i]=1;
            }
            else if(traceLine.at(7).toInt()==BNBScheduler::Events::SENT)
            {
                currentProc=traceLine.at(1).toInt();
                sents[currentProc]=traceLine.at(0).toInt();
                for (int i=sends[currentProc];i<=sents[currentProc];i++) procs[currentProc].sending[i]=1;
            }
            else if(traceLine.at(7).toInt()==BNBScheduler::Events::COMMAND_ARRIVED ||
                    traceLine.at(7).toInt()==BNBScheduler::Events::DATA_ARRIVED)
            {
                currentProc=traceLine.at(1).toInt();
                arrives[currentProc]=traceLine.at(0).toInt();
                for (int i=recvs[currentProc];i<arrives[currentProc];i++) procs[currentProc].receiving[i]=1;
            }
        }
        //reading actions
        if(traceLine.length()>2)
        {
            if(traceLine.at(2).toInt()==BNBScheduler::Actions::SOLVE)
            {
                if(!procNumSet && traceLine.at(0).toInt()==0 && traceLine.at(1).toInt()==0) steps=traceLine.at(3).toInt();
                currentProc=traceLine.at(1).toInt();
                solves[currentProc]=traceLine.at(0).toInt();
                //for (int i=dones[currentProc];i<solves[currentProc];i++) procs[currentProc].activity[i]=0;
            }
            else if(traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_COMMAND ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_RECORDS ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_SUB ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_SUB_AND_RECORDS)
            {
                currentProc=traceLine.at(1).toInt();
                sends[currentProc]=traceLine.at(0).toInt();
                //for (int i=sents[currentProc];i<sends[currentProc];i++) procs[currentProc].sending[i]=0;
            }
            else if(traceLine.at(2).toInt()==BNBScheduler::Actions::RECV)
            {
                currentProc=traceLine.at(1).toInt();
                recvs[currentProc]=traceLine.at(0).toInt();
                //for (int i=arrives[currentProc];i<recvs[currentProc];i++) procs[currentProc].receiving[i]=0;
            }
        }

    }
    if(!procNumSet)
    {
        ui->StepsEdit->setText(QString::number(steps));
        ui->ProcNumEdit->setText(QString::number(procNum));
    }

    /*
     * cleaning old visualization if it exists
     */
    if(ui->plotWidget->layout()!=NULL)
    {
        delete(ui->plotWidget->layout());

    }
    if(ui->gridProcLayout->itemAt(0)!=NULL)
    {
        QWidget *w=ui->gridProcLayout->itemAt(0)->widget();
        ui->exchangeLayout->removeWidget(w);
        delete(w);
    }
    if(ui->exchangeLayout->itemAt(0)!=NULL)
    {
        QWidget *w=ui->exchangeLayout->itemAt(0)->widget();
        ui->exchangeLayout->removeWidget(w);
        delete(w);
    }
    setUpdatesEnabled(false);
    this->repaint();
    setUpdatesEnabled(true);

    /*
     * preparing plots tab
     */
    PlotLayout *pl = new PlotLayout(procNum, CURVE_MIN_LENGTH, PLOT_MAX_SIZE, QApplication::desktop()->width());
    ui->plotWidget->setLayout(pl);
    plots=pl->plots;
    activityCurves= pl->activityCurves;
    sendingCurves= pl->sendingCurves;
    receivingCurves= pl->receivingCurves;
    time.resize(CURVE_MIN_LENGTH);
    subproc.activity.resize(CURVE_MIN_LENGTH);
    subproc.sending.resize(CURVE_MIN_LENGTH);
    subproc.receiving.resize(CURVE_MIN_LENGTH);
    time.squeeze();
    subproc.activity.squeeze();
    subproc.sending.squeeze();
    subproc.receiving.squeeze();
    /*
     * preparing processes grid tab
     */
    GridProcView *gridProcView = new GridProcView(procNum,RECT_SIZE);
    rects=gridProcView->rects;
    ui->gridProcLayout->addWidget(gridProcView);

    /*
     * preparing exchange tab
     */
    ExchangeView *exchangeView = new ExchangeView(procNum,RECT_SIZE);
    senders=exchangeView->senders;
    receivers=exchangeView->receivers;
    ui->exchangeLayout->addWidget(exchangeView);
    /*
     * preparing control widget
     */
    ui->PlayButton->setEnabled(true);
    ui->PauseButton->setEnabled(true);
    ui->StopButton->setEnabled(true);
    ui->XScaleSlider->setEnabled(true);
    ui->XScaleSlider->setValue(0);
    ui->XScaleSlider->setMaximum(maxTime+1-CURVE_MIN_LENGTH);
    ui->horizontalSlider->setEnabled(true);
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider->setMaximum(maxTime);
    ui->DTSlider->setEnabled(true);
    ui->DTSlider->setValue(1);

}
