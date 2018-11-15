TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += mainwindow.h   chartwindow.h   configurationeditor.h   mdiarea.h   chartwidget.h
SOURCES += mainwindow.cpp chartwindow.cpp configurationeditor.cpp mdiarea.cpp chartwidget.cpp
FORMS	=  mainwindow.ui  chartwindow.ui  configurationeditor.ui

include(logdoc/logdoc.pri)
include(series/series.pri)
