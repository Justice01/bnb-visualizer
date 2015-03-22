#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    procs=NULL;
    plots=NULL;
}

MainWindow::~MainWindow()
{
    if (procs!=NULL) delete [] procs;
    delete ui;
}

void MainWindow::on_CountButton_clicked()
{
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
    killTimer(timerID);
    ui->textEdit->clear();
    if(ui->plotWidget->layout()!=NULL)
    {
        if (procs!=NULL) delete [] procs;
        procs=NULL;
        delete(ui->plotWidget->layout());
        setUpdatesEnabled(false);
        this->repaint();
        setUpdatesEnabled(true);
    }
    int procNum = ui->ProcNumEdit->text().toInt();
    PlotLayout *pl = new PlotLayout(procNum, CURVE_LENGTH);
    ui->plotWidget->setLayout(pl);
    plots=pl->plots;//((PlotLayout*)(ui->plotWidget->layout()))->plots;
    curves= pl->curves;
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
    ui->textEdit->setText(QString::fromLocal8Bit(ar));
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->maximum());

    procs = new processor[procNum];
    int currentProc=0;
    int maxTime = trace.last().split(' ').first().toInt();
    int solves[procNum];
    int dones[procNum];
    for (int i=0; i<procNum;i++)
    {
        solves[i]=0;
        dones[i]=0;
    }
    for (int i=0; i<procNum;i++)
    {
        procs[i].activity= new int[maxTime+1];
        for (int j=0;j<=maxTime;j++)procs[i].activity[j]=0;
    }
    QStringList traceLine;
    for (int j=0; j<trace.length();j++)
    {
        traceLine = trace[j].split(' ');
        if(traceLine.length()>2)
            if(traceLine.at(2).toInt()==9)
            {
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
    //QMessageBox::information(this,"",QString::number(traceLine[1].toInt()));
    ui->PlayButton->setEnabled(true);
    ui->PauseButton->setEnabled(true);
    ui->StopButton->setEnabled(true);
    ui->horizontalSlider->setEnabled(true);
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider->setMaximum(maxTime);
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if (ui->DTEdit->text().toInt()!=0)
        if(ui->horizontalSlider->value() < ui->horizontalSlider->maximum())ui->horizontalSlider->setValue(ui->horizontalSlider->value()+ui->DTEdit->text().toInt());
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
    for(int j=0;j < plots->length();j++)
    {
        plots->at(j)->setAxisScale( QwtPlot::xBottom, value - CURVE_LENGTH, value);
        int x = value;
        for(int i=CURVE_LENGTH-1;i>=0;i--)
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
        curves->at(j)->setSamples(time, activity, CURVE_LENGTH );
        plots->at(j)->replot();
    }
}
