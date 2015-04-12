#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plots=NULL;
    curves=NULL;
    maxTime=0;
    procNum=0;
    connect(ui->actionLoad_trace,SIGNAL(triggered()),this,SLOT(loadTrace()));
}

MainWindow::~MainWindow()
{
    //if(scene!=NULL) delete scene;
    delete ui;
}

void MainWindow::on_ComputeButton_clicked()
{
    //stop timer if started
    killTimer(timerID);
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

    //parsing data from trace
    parseTrace(trace,ui->ProcNumEdit->text().toInt());
    //prepare plots tab
    preparePlots();
    //prepare processors grid tab
    prepareGridProc();
    //prepare scene for data exchange visualization tab
    prepareExchange();
    //prepare control widget
    prepareControlWidget();
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
                    activity[i] = procs[j].activity[x];
                }
                else
                {
                    time[i]=0.0;
                    activity[i] = 0.0;
                }
                x--;
            }
            curves->at(j)->setSamples(time, activity);
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
            else if(procs[i].activity[value]==0)
            {
                rects->at(i)->setBrush(Qt::gray);
            }
        }
    }
    if(ui->tabWidget->currentWidget()==ui->ExchangeTab)
    {

    }
}

void MainWindow::on_XScaleSlider_valueChanged(int value)
{
    time.resize(CURVE_MIN_LENGTH+value);
    activity.resize(CURVE_MIN_LENGTH+value);
    updatePlots(ui->horizontalSlider->value());
}

void MainWindow::loadTrace()
{
    //stop timer if started
    killTimer(timerID);
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
    parseTrace(trace);
    //prepare plots tab
    preparePlots();
    //prepare processors grid tab
    prepareGridProc();
    //prepare scene for data exchange visualization tab
    prepareExchange();
    //prepare control widget
    prepareControlWidget();
}

void MainWindow::parseTrace(QStringList &trace, int procNum)
{
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
    for (int i=0; i<procNum;i++)
    {
        solves[i]=0;
        dones[i]=0;
        procs[i].activity.resize(maxTime+1);
        procs[i].activity.squeeze();
        for (int j=0;j<=maxTime;j++)procs[i].activity[j]=0;
    }

    for (int j=0; j<trace.length();j++)
    {
        traceLine = trace[j].split(' ');
        if(traceLine.length()>2)
            if(traceLine.at(2).toInt()==9)
            {
                if(!procNumSet && traceLine.at(0).toInt()==0) steps=traceLine.at(3).toInt();
                currentProc=traceLine.at(1).toInt();
                solves[currentProc]=traceLine.at(0).toInt();
                for (int i=dones[currentProc];i<solves[currentProc];i++) procs[currentProc].activity[i]=0;
            }
        if(traceLine.length()>7)
            if(traceLine.at(7).toInt()==2)
            {
                currentProc=traceLine.at(1).toInt();
                dones[currentProc]=traceLine.at(0).toInt();
                for (int i=solves[currentProc];i<dones[currentProc];i++) procs[currentProc].activity[i]=1;
            }
    }
    if(!procNumSet)
    {
        ui->StepsEdit->setText(QString::number(steps));
        ui->ProcNumEdit->setText(QString::number(procNum));
    }
}
void MainWindow::preparePlots()
{
    //preparing plots tab
    if(ui->plotWidget->layout()!=NULL)
    {
        delete(ui->plotWidget->layout());
        setUpdatesEnabled(false);
        this->repaint();
        setUpdatesEnabled(true);
    }

    PlotLayout *pl = new PlotLayout(procNum, CURVE_MIN_LENGTH, PLOT_MAX_SIZE, QApplication::desktop()->width());
    ui->plotWidget->setLayout(pl);
    plots=pl->plots;
    curves= pl->curves;
    // initializing curves data
    time.resize(CURVE_MIN_LENGTH);
    activity.resize(CURVE_MIN_LENGTH);
    time.squeeze();
    activity.squeeze();
}

void MainWindow::prepareGridProc()
{
    //preparing processes grid tab
    if(ui->gridProcLayout->itemAt(0)!=NULL)
    {
        QWidget *w=ui->gridProcLayout->itemAt(0)->widget();
        ui->exchangeLayout->removeWidget(w);
        delete(w);
        setUpdatesEnabled(false);
        this->repaint();
        setUpdatesEnabled(true);
    }
    GridProcView *view = new GridProcView(procNum,RECT_SIZE,QApplication::desktop()->width());
    rects=view->rects;
    ui->gridProcLayout->addWidget(view);
}

void MainWindow::prepareExchange()
{
    //preparing exchange tab
    if(ui->exchangeLayout->itemAt(0)!=NULL)
    {
        QWidget *w=ui->exchangeLayout->itemAt(0)->widget();
        ui->exchangeLayout->removeWidget(w);
        delete(w);
        setUpdatesEnabled(false);
        this->repaint();
        setUpdatesEnabled(true);
    }
    ExchangeView *view = new ExchangeView(procNum,RECT_SIZE,QApplication::desktop()->width());
    ui->exchangeLayout->addWidget(view);
}
void MainWindow::prepareControlWidget()
{
    //preparing control widget
    ui->PlayButton->setEnabled(true);
    ui->PauseButton->setEnabled(true);
    ui->StopButton->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider->setMaximum(maxTime);
    ui->DTSlider->setEnabled(true);
    ui->DTSlider->setValue(1);
    ui->XScaleSlider->setEnabled(true);
    ui->XScaleSlider->setValue(0);
    ui->XScaleSlider->setMaximum(maxTime+1-CURVE_MIN_LENGTH);
}
