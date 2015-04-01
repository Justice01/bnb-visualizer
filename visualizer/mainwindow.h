#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotlayout.h"
#include <qscrollarea.h>
#include <qprocess.h>
//#include <qiodevice.h>
//#include <qtextcodec.h>
#include <qscrollbar.h>
#include "processor.h"
#include <qmessagebox.h>
#include <qdesktopwidget.h>
//#include <stdlib.h>

#define CURVE_MIN_LENGTH 100
#define PLOT_MAX_SIZE 200
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //virtual void mouseMoveEvent(QMouseEvent *event);
    //virtual void mousePressEvent(QMouseEvent *event);
private slots:
    void on_ComputeButton_clicked();

    void on_PlayButton_clicked();

    void on_StopButton_clicked();

    void on_PauseButton_clicked();

    void on_horizontalSlider_valueChanged(int value);


    void on_XScaleSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    processor *procs;
    QList<QwtPlot*> *plots;
    QList<QwtPlotCurve*> *curves;
    int timerID;
    double * time;
    double * activity;
    //int xClickPosition;
    //int yClickPosition;
    void updatePlots(int value);

protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // MAINWINDOW_H
