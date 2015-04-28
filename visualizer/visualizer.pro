#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T23:52:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BnBVisualizer
TEMPLATE = app
RESOURCES = \
    myres.qrc
SOURCES += main.cpp\
        mainwindow.cpp \
    plotlayout.cpp \
    exchangeview.cpp \
    exchanger.cpp \
    tableprocview.cpp \
    helpdialog.cpp \
    paintwidget.cpp

HEADERS  += mainwindow.h \
    plotlayout.h \
    processor.h \
    exchangeview.h \
    bnbscheduler.hpp \
    solverinfo.hpp \
    exchanger.h \
    tableprocview.h \
    helpdialog.h \
    paintwidget.h

FORMS    += mainwindow.ui \
    helpdialog.ui

unix:!macx: LIBS += -L$$OUT_PWD/qwt/lib/ -lqwt
QMAKE_LFLAGS+= '-Wl,-rpath,$$OUT_PWD/qwt/lib' '-Wl,--rpath-link,$$OUT_PWD/qwt/lib'
INCLUDEPATH += $$PWD/qwt/src
DEPENDPATH += $$PWD/qwt/src
