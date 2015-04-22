#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T23:52:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BnBVisualizer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    plotlayout.cpp \
    exchangeview.cpp \
    gridprocview.cpp \
    exchanger.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    plotlayout.h \
    processor.h \
    exchangeview.h \
    gridprocview.h \
    bnbscheduler.hpp \
    solverinfo.hpp \
    exchanger.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui

unix:!macx: LIBS += -L$$OUT_PWD/qwt/lib/ -lqwt
QMAKE_LFLAGS+= '-Wl,-rpath,$$OUT_PWD/qwt/lib' '-Wl,--rpath-link,$$OUT_PWD/qwt/lib'
INCLUDEPATH += $$PWD/qwt/src
DEPENDPATH += $$PWD/qwt/src
