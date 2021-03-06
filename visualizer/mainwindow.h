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
#include "tableprocview.h"
#include "bnbscheduler.hpp"
#include "exchanger.h"
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>
#include <qdatetime.h>
#include "helpdialog.h"
#include <qtextedit.h>
#include <qwt_legend.h>
#include "procresult.h"
#include <qgroupbox.h>

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
    void on_tabWidget_currentChanged(int index);

    void on_RewindButton_clicked();

    void on_ForwardButton_clicked();

public slots:
    void loadTrace();
    void viewHelp();
private:
    Ui::MainWindow *ui;
    QVector<processor> procs;
    QVector<QwtPlot*> *plots;
    QVector<QwtPlotCurve*> *activityCurves;
    QVector<QwtPlotCurve*> *sendingCurves;
    QVector<QwtPlotCurve*> *receivingCurves;
    QVector<QGraphicsRectItem*> *rects;
    QVector<QGraphicsRectItem*> *senders;
    QVector<QGraphicsRectItem*> *receivers;
    QList<exchanger*> *exchanges;
    QwtPlot *statisticsPlot;
    QwtPlotCurve *statisticsCurve;
    QwtPlotCurve *peakPerformanceCurve;
    QTextEdit *statisticsEdit;
    int timerID;
    int procNum;
    int maxTime;
    QVector<double> time;
    processor subproc;
    void prepareVisualization(QStringList &trace, int procNum=0);
    void updatePlots(int value);
    void parseTrace(QStringList &trace, int procNum=0);
    void jsonLoad(QString fileName);
    void jsonSave(QString fileName);

    QVector<QGroupBox*> jsonToWidgets(QJsonObject &obj);
    QVBoxLayout* jsonObjectToVBoxLayout(QJsonObject &obj);
    QJsonObject widgetsToJson(QVector<QGroupBox *> gBoxes);
    QJsonObject gBoxLayoutToJson(QGroupBox *gBox);
protected:
    virtual void timerEvent(QTimerEvent *);
    virtual void closeEvent(QCloseEvent * event);
};

#endif // MAINWINDOW_H
