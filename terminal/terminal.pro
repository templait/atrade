TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += mainwindow.h   chartwindow.h   configurationeditor.h   chartwidget.h   configurationmodel.h
SOURCES += mainwindow.cpp chartwindow.cpp configurationeditor.cpp chartwidget.cpp configurationmodel.cpp
FORMS	=  mainwindow.ui  chartwindow.ui  configurationeditor.ui

include(logdoc/logdoc.pri)
include(series/series.pri)
