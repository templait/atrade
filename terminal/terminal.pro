TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += mainwindow.h   chartwindow.h   chartwidget.h
SOURCES += mainwindow.cpp chartwindow.cpp chartwidget.cpp
FORMS	=  mainwindow.ui  chartwindow.ui

include(logdoc/logdoc.pri)
include(series/series.pri)
