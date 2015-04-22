#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotlayout.h"
#include <qscrollarea.h>
#include <qprocess.h>
#include <qscrollbar.h>
#include "processor.h"
#include <qmessagebox.h>
#include <qdesktopwidget.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qgraphicsitem.h>
#include <qfiledialog.h>
#include "exchangeview.h"
#include "gridprocview.h"
#include "bnbscheduler.hpp"
#include "exchanger.h"

#define CURVE_MIN_LENGTH 100
#define PLOT_MAX_SIZE 250
#define RECT_SIZE 10
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_ComputeButton_clicked();

    void on_PlayButton_clicked();

    void on_StopButton_clicked();

    void on_PauseButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_XScaleSlider_valueChanged(int value);
public slots:
    void loadTrace();
private:
    Ui::MainWindow *ui;
    QVector<processor> procs;
    QList<QwtPlot*> *plots;
    QList<QwtPlotCurve*> *activityCurves;
    QList<QwtPlotCurve*> *sendingCurves;
    QList<QwtPlotCurve*> *receivingCurves;
    QList<QGraphicsRectItem*> *rects;
    QVector<QGraphicsRectItem*> *senders;
    QVector<QGraphicsRectItem*> *receivers;
    QList<exchanger*> *exchanges;
    int timerID;
    int procNum;
    int maxTime;
    QVector<double> time;
    /*QVector<double> activity;
    QVector<double> sending;
    QVector<double> receiving;*/
    processor subproc;
    void prepareVisualization(QStringList &trace, int procNum=0);
    void updatePlots(int value);
    void parseTrace(QStringList &trace, int procNum=0);
protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // MAINWINDOW_H
