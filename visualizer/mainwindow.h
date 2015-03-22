#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotlayout.h"
#include <qscrollarea.h>
#include <qprocess.h>
#include <qiodevice.h>
#include <qtextcodec.h>
#include <qscrollbar.h>
#include "processor.h"
#include <qmessagebox.h>
#include <stdlib.h>

#define CURVE_LENGTH 100
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
    void on_CountButton_clicked();

    void on_PlayButton_clicked();

    void on_StopButton_clicked();

    void on_PauseButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    processor *procs;
    QList<QwtPlot*> *plots;
    QList<QwtPlotCurve*> *curves;
    int timerID;
    double time[CURVE_LENGTH];
    double activity[CURVE_LENGTH];

protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // MAINWINDOW_H
