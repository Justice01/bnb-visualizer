#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry((QApplication::desktop()->rect().width() - this->rect().width())/2,(QApplication::desktop()->rect().height() - this->rect().height())/2,this->rect().width(),this->rect().height());
    plots=NULL;
    activityCurves=NULL;
    sendingCurves=NULL;
    receivingCurves=NULL;
    rects=NULL;
    exchanges=NULL;
    maxTime=0;
    procNum=0;
    connect(ui->actionLoad_trace,SIGNAL(triggered()),this,SLOT(loadTrace()));
    connect(ui->actionView_Help,SIGNAL(triggered()),this,SLOT(viewHelp()));
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->timeLabel,SLOT(setNum(int)));
    jsonLoad("settings.json");
    ui->StatisticsTab->setLayout(new QVBoxLayout());
    ui->StatisticsTab->layout()->addWidget(new QLabel("Processors usage:"));
    statisticsPlot =  new QwtPlot();
    statisticsPlot->setMaximumHeight(PLOT_MAX_SIZE);
    QwtLegend *leg=new QwtLegend();
    statisticsPlot->insertLegend(leg,QwtPlot::TopLegend);
    ui->StatisticsTab->layout()->addWidget(statisticsPlot);
    statisticsEdit = new QTextEdit();
    statisticsEdit->setReadOnly(true);
    statisticsEdit->setFont(QFont("",14,10));
    ui->StatisticsTab->layout()->addWidget(statisticsEdit);
    peakPerformanceCurve= new QwtPlotCurve("Peak performance");
    peakPerformanceCurve->setPen(Qt::green);
    peakPerformanceCurve->setBrush(Qt::green);
    peakPerformanceCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    peakPerformanceCurve->attach(statisticsPlot);
    statisticsCurve= new QwtPlotCurve("Real performance");
    statisticsCurve->setPen(Qt::blue);
    statisticsCurve->setBrush(Qt::blue);
    statisticsCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    statisticsCurve->attach(statisticsPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ComputeButton_clicked()
{
    jsonSave("settings.json");
    //preparing input data
    if(ui->ProcNumEdit->text().toInt()==0)
    {
        QMessageBox::information(this,"","Wrong process number!");
        return;
    }
    /*if(ui->InputEdit->toPlainText().toInt()==0)
    {
        QMessageBox::information(this,"","Wrong step!");
        return;
    }*/
    if(ui->storeEdit->text().isEmpty()||
            ui->solveEdit->text().isEmpty()||
            ui->overheadEdit->text().isEmpty()||
            ui->loadEdit->text().isEmpty()||
            ui->subproblemEdit->text().isEmpty()||
            ui->recordEdit->text().isEmpty()||
            ui->commandEdit->text().isEmpty()||
            ui->bandwidthEdit->text().isEmpty()||
            ui->latencyEdit->text().isEmpty()||
            ui->maxTaskLevelEdit->text().isEmpty())
    {
        QMessageBox::information(this,"","Wrong settings!");
        return;
    }
    if(ui->pathEdit->text().isEmpty())
    {
        QString s;
        QDateTime d = QDateTime::currentDateTime();
        int dateTimeItem=d.date().year();
        s.append(QString::number(dateTimeItem));
        dateTimeItem=d.date().month();
        s.append(dateTimeItem>10?QString::number(dateTimeItem):("0"+QString::number(dateTimeItem)));
        dateTimeItem=d.date().day();
        s.append(dateTimeItem>10?QString::number(dateTimeItem):("0"+QString::number(dateTimeItem)));
        dateTimeItem=d.time().hour();
        s.append(dateTimeItem>10?QString::number(dateTimeItem):("0"+QString::number(dateTimeItem)));
        dateTimeItem=d.time().minute();
        s.append(dateTimeItem>10?QString::number(dateTimeItem):("0"+QString::number(dateTimeItem)));
        dateTimeItem=d.time().second();
        s.append(dateTimeItem>10?QString::number(dateTimeItem):("0"+QString::number(dateTimeItem)));
        ui->pathEdit->setText(s);
    }
    //reading bnb-simulators data (trace)
    QByteArray ar;
    QProcess *p = new QProcess();
    p->setReadChannelMode(QProcess::MergedChannels);
    p->start("./bnbtest");
    p->write(QString(ui->ProcNumEdit->text()+" "+ui->InputEdit->toPlainText()).toLocal8Bit());
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
    QDir dir;
    if(!dir.exists("traces")){dir.mkdir("traces");}
    QFile f("traces/"+ui->pathEdit->text()+".trc");
    if(f.open(QFile::ReadWrite|QFile::Truncate))
    {
        f.write(ar);
        f.close();
    }
    else
    {
        QMessageBox::information(this,"","Error while saving trace!");
        return;
    }

    prepareVisualization(trace,ui->ProcNumEdit->text().toInt());
    QMessageBox::information(this,"","Done!");
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
    if(ui->tabWidget->currentWidget()==ui->TableProcTab)
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
        for (int i=0;i<rects->length();i++)
        {
            if(procs[i].activity[value]==1)
            {
                senders->at(i)->setBrush(Qt::blue);
                receivers->at(i)->setBrush(Qt::blue);
            }
            else if(procs[i].sending[value]==1)
            {
                senders->at(i)->setBrush(Qt::green);
                receivers->at(i)->setBrush(Qt::green);
            }
            else if(procs[i].receiving[value]==1)
            {
                senders->at(i)->setBrush(Qt::red);
                receivers->at(i)->setBrush(Qt::red);
            }
            else
            {
                senders->at(i)->setBrush(Qt::gray);
                receivers->at(i)->setBrush(Qt::gray);
            }
        }
        QListIterator<exchanger*>it(*exchanges);
        while(it.hasNext()) it.next()->drawLines(value,senders,receivers);
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
    if(ui->PauseButton->isEnabled())ui->PauseButton->click();
    QDir dir;
    if(!dir.exists("traces")){dir.mkdir("traces");}
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Open file"),
                                QDir::currentPath()+"/traces",
                                tr("Trace (*.trc)"));
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
    QMessageBox::information(this,"","Done!");
}

void MainWindow::prepareVisualization(QStringList&trace, int procNum)
{
    //stop timer if started
    killTimer(timerID);
    /*
     * parcing trace
     */
    QStringList traceLine;
    QStringListIterator traceIt(trace);
    bool procNumSet=true;
    if (procNum==0)procNumSet=false;
    //if second parametr 0 determine procNum from trace
    if (!procNumSet)
    {
        while(traceIt.hasNext())
        {
            traceLine=traceIt.next().split(' ');
            if(traceLine.length()>2)
            {
                if(traceLine.at(1).toInt()>=procNum && traceLine.at(2).toInt()==6) procNum=traceLine.at(1).toInt()+1;
                else if(traceLine.at(0).toInt()>0) break;
            }
        }
    }
    this->procNum=procNum;
    procs.resize(procNum);
    procs.squeeze();
    int currentProc=0;
    maxTime = trace.last().split(' ').first().toInt();
    int solves[procNum];
    int dones[procNum];
    int sends[procNum];
    int sents[procNum];
    int recvs[procNum];
    int arrives[procNum];
    if(exchanges!=NULL)
    {
        QListIterator<exchanger*>it(*exchanges);
        while(it.hasNext()) delete it.next();
        delete exchanges;
        exchanges=NULL;
    }
    exchanges = new QList<exchanger*>();
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
    traceIt=QStringListIterator(trace);
    //reading trace
    while(traceIt.hasNext())
    {
        traceLine = traceIt.next().split(' ');
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
                QList<exchanger*>::iterator it=exchanges->end();
                while(true)
                {
                    --it;
                    if((*it)->getTo()==currentProc && (*it)->getFrom()==traceLine.at(8).toInt() && (*it)->getEnd()==0)
                    {
                        (*it)->setEnd(arrives[currentProc]);
                        break;
                    }
                    if(it==exchanges->begin()) break;
                }
            }
        }
        //reading actions
        if(traceLine.length()>2)
        {
            if(traceLine.at(2).toInt()==BNBScheduler::Actions::SOLVE)
            {
                currentProc=traceLine.at(1).toInt();
                solves[currentProc]=traceLine.at(0).toInt();
            }
            else if(traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_COMMAND ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_RECORDS ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_SUB ||
                    traceLine.at(2).toInt()==BNBScheduler::Actions::SEND_SUB_AND_RECORDS)
            {
                currentProc=traceLine.at(1).toInt();
                sends[currentProc]=traceLine.at(0).toInt();
                exchanges->append(new exchanger(currentProc,traceLine.at(3).toInt(),NULL,sends[currentProc],0));
            }
            else if(traceLine.at(2).toInt()==BNBScheduler::Actions::RECV)
            {
                currentProc=traceLine.at(1).toInt();
                recvs[currentProc]=traceLine.at(0).toInt();
            }
        }

    }
    for (int i=0;i<procNum;i++)
    {
        if (solves[i]>dones[i])
            for (int j=solves[i];j<=maxTime;j++) procs[i].activity[j]=1;
        else if(recvs[i]>arrives[i])
            for (int j=recvs[i];j<=maxTime;j++) procs[i].receiving[j]=1;
        else if(sends[i]>sents[i])
            for (int j=sends[i];j<=maxTime;j++) procs[i].sending[j]=1;

    }

    /*
     * cleaning old visualization if it exists
     */
    if(ui->plotWidget->layout()!=NULL)
    {
        delete(ui->plotWidget->layout());

    }
    if(ui->tableProcLayout->itemAt(0)!=NULL)
    {
        QWidget *w=ui->tableProcLayout->itemAt(0)->widget();
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
     * preparing processes table tab
     */
    TableProcView *tableProcView = new TableProcView(procNum,RECT_SIZE);
    rects=tableProcView->rects;
    ui->tableProcLayout->addWidget(tableProcView);

    /*
     * preparing exchange tab
     */
    ExchangeView *exchangeView = new ExchangeView(procNum,RECT_SIZE);
    senders=exchangeView->senders;
    receivers=exchangeView->receivers;
    ui->exchangeLayout->addWidget(exchangeView);
    if(exchanges!=NULL)
    {
        QListIterator<exchanger*>it(*exchanges);
        while(it.hasNext()) it.next()->addScene(exchangeView->scene);
    }

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

    /*
     * preparing statistics
     */
    //accumulate processors data
    QVector<double> realPerformance(maxTime+1);
    QVector<double> peakPerformance(maxTime+1);
    QVector<double> parallelTime(maxTime+1);
    QVector<procresult> results(procNum);
    bool allInReceive=true;
    bool existsExchange=false;
    for (int i=0;i<realPerformance.size();i++)
    {
        parallelTime[i]=i;
        realPerformance[i]=0;
        peakPerformance[i]=procNum;
        for(int j=0;j<procNum;j++)
        {
            realPerformance[i]+=procs[j].activity[i];
            results[j].activity+=procs[j].activity[i];
            results[j].receiving+=procs[j].receiving[i];
            results[j].sending+=procs[j].sending[i];
            if(i==realPerformance.size()-1)
            {
                results[j].activityPercent=(results[j].activity/i)*100;
                if(procs[j].receiving[i]!=1)allInReceive=false;
            }
        }
    }
    QListIterator<exchanger*> it(*exchanges);
    while(it.hasNext())
    {
        if(it.next()->getEnd()==0)existsExchange=true;
    }

    //updating plot
    statisticsPlot->setAxisScale(QwtPlot::xBottom,0,parallelTime.size(),0);
    statisticsPlot->setAxisScale(QwtPlot::yLeft,0,procNum,0);
    peakPerformanceCurve->setSamples(parallelTime,peakPerformance);
    statisticsCurve->setSamples(parallelTime,realPerformance);
    statisticsPlot->replot();
    statisticsEdit->clear();
    //axeleration and efficiency count
    int sequentalTime=0;
    for (int i=0;i<realPerformance.size();i++)
    {
        sequentalTime+=realPerformance[i];
    }
    double axeleration=((double)sequentalTime)/parallelTime.last();
    double efficiency=axeleration/procNum;
    //total statistics
    if(!existsExchange && allInReceive)
    {
        statisticsEdit->setTextColor(QColor(Qt::red));
        statisticsEdit->insertPlainText(QString("DEADLOCK!\n"));
    }
    if(axeleration<1.0) statisticsEdit->setTextColor(QColor(Qt::red));
    else statisticsEdit->setTextColor(QColor(Qt::black));
    statisticsEdit->insertPlainText(QString("Speedup is ")+QString::number(axeleration)+"\n");
    statisticsEdit->setTextColor(QColor(Qt::black));
    statisticsEdit->insertPlainText(QString("Efficiency is ")+QString::number(efficiency)+"\n");
    //individual processors statistics
    for(int i=0;i<procNum;i++)
    {
        statisticsEdit->setTextColor(QColor(Qt::black));
        statisticsEdit->insertPlainText(QString("\n-----Process ")+QString::number(i)+"-----\n");
        if(results[i].activityPercent<50) statisticsEdit->setTextColor(QColor(Qt::red));
        else statisticsEdit->setTextColor(QColor(Qt::black));
        statisticsEdit->insertPlainText(QString("Solving time in percent is ")+QString::number(results[i].activityPercent)+"\%\n");
        statisticsEdit->insertPlainText(QString("Solving time is ")+QString::number(results[i].activity)+"\n");
        statisticsEdit->insertPlainText(QString("Sending time is ")+QString::number(results[i].sending)+"\n");
        statisticsEdit->insertPlainText(QString("Receiving time is ")+QString::number(results[i].receiving)+"\n");
    }

}
void MainWindow::jsonLoad(QString fileName)
{
    QFile f(fileName);
    if(f.open(QIODevice::ReadOnly))
    {
        QJsonObject obj=QJsonDocument::fromJson(((QString)f.readAll()).toUtf8()).object();
        ui->solveEdit->setText(QString::number(((obj.value("timer").toObject()).value("time cost").toObject())["solve"].toDouble()));
        ui->storeEdit->setText(QString::number(((obj.value("timer").toObject()).value("time cost").toObject())["store"].toDouble()));
        ui->loadEdit->setText(QString::number(((obj.value("timer").toObject()).value("time cost").toObject())["load"].toDouble()));
        ui->overheadEdit->setText(QString::number(((obj.value("timer").toObject()).value("time cost").toObject())["overhead"].toDouble()));
        ui->commandEdit->setText(QString::number(((obj.value("serializer").toObject()).value("parcel size").toObject())["command"].toDouble()));
        ui->recordEdit->setText(QString::number(((obj.value("serializer").toObject()).value("parcel size").toObject())["record"].toDouble()));
        ui->subproblemEdit->setText(QString::number(((obj.value("serializer").toObject()).value("parcel size").toObject())["subproblem"].toDouble()));
        ui->latencyEdit->setText(QString::number((obj.value("communicator").toObject())["latency"].toDouble()));
        ui->bandwidthEdit->setText(QString::number((obj.value("communicator").toObject())["bandwidth"].toDouble()));
        ui->maxTaskLevelEdit->setText(QString::number((obj.value("resolver").toObject())["maximal task level"].toDouble()));
    }
    else QMessageBox::information(this,"","settings file was not load!");
    f.close();
}

void MainWindow::jsonSave(QString fileName)
{
    QFile f(fileName);
    if(f.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        QJsonObject obj;

        QJsonObject timer;
        QJsonObject time_cost;

        QJsonObject serializer;
        QJsonObject parcel_size;

        QJsonObject communicator;

        QJsonObject resolver;

        time_cost["solve"]=ui->solveEdit->text().toInt();
        time_cost["store"]=ui->storeEdit->text().toInt();
        time_cost["load"]=ui->loadEdit->text().toInt();
        time_cost["overhead"]=ui->overheadEdit->text().toInt();

        timer["time cost"]=time_cost;

        parcel_size["command"]=ui->commandEdit->text().toInt();
        parcel_size["record"]=ui->recordEdit->text().toInt();
        parcel_size["subproblem"]=ui->subproblemEdit->text().toInt();

        serializer["parcel size"]=parcel_size;

        communicator["latency"]=ui->latencyEdit->text().toInt();
        communicator["bandwidth"]=ui->bandwidthEdit->text().toInt();

        resolver["maximal task level"]=ui->maxTaskLevelEdit->text().toInt();

        obj["timer"]=timer;
        obj["serializer"]=serializer;
        obj["communicator"]=communicator;
        obj["resolver"]=resolver;
        QJsonDocument doc(obj);
        f.write(doc.toJson());
    }
    else QMessageBox::information(this,"","settings file was not save!");
    f.close();
}
void MainWindow::closeEvent(QCloseEvent * event)
{
    jsonSave("settings.json");
    event->accept();
}

void MainWindow::viewHelp()
{
    if(ui->PauseButton->isEnabled())ui->PauseButton->click();
    HelpDialog::TabType type=HelpDialog::PLOT;
    if(ui->tabWidget->currentWidget()==ui->PlotTab)
    {
        type=HelpDialog::PLOT;
    }
    else if(ui->tabWidget->currentWidget()==ui->TableProcTab)
    {
        type=HelpDialog::TABLE;
    }
    else if(ui->tabWidget->currentWidget()==ui->ExchangeTab)
    {
        type=HelpDialog::EXCHANGE;
    }
    HelpDialog dialog(type,this);
    dialog.exec();

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->horizontalSlider->isEnabled())ui->horizontalSlider->valueChanged(ui->horizontalSlider->value());
}
