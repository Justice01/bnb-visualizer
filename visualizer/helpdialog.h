#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <qpainter.h>
#include "paintwidget.h"
#include "mainwindow.h"
#include <qlabel.h>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    enum  TabType
    {
        PLOT,
        TABLE,
        EXCHANGE
    };
    explicit HelpDialog(QWidget *parent = 0);
    explicit HelpDialog(TabType type, QWidget *parent = 0);

    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
    QList<QLabel*> *lbls;
    QList<PaintWidget*> *wdgts;
    void fillHelp();
};

#endif // HELPDIALOG_H
