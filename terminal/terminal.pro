TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += mainwindow.h   chartwindow.h   mdiarea.h   chartwidget.h
SOURCES += mainwindow.cpp chartwindow.cpp mdiarea.cpp chartwidget.cpp
FORMS	=  mainwindow.ui  chartwindow.ui

HEADERS += configurationeditor.h   titleconfigurationeditor.h
SOURCES += configurationeditor.cpp titleconfigurationeditor.cpp
FORMS	+= configurationeditor.ui  titleconfigurationeditor.ui

include(logdoc/logdoc.pri)
include(series/series.pri)
