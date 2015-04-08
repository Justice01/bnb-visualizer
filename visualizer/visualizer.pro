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
    exchangescene.cpp

HEADERS  += mainwindow.h \
    plotlayout.h \
    processor.h \
    exchangescene.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$OUT_PWD/qwt/lib/ -lqwt
QMAKE_LFLAGS+= '-Wl,-rpath,$$OUT_PWD/qwt/lib' '-Wl,--rpath-link,$$OUT_PWD/qwt/lib'
INCLUDEPATH += $$PWD/qwt/src
DEPENDPATH += $$PWD/qwt/src
