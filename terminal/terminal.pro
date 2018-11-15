TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += mainwindow.h   chartwindow.h   mdiarea.h   chartwidget.h   timeintervalcombobox.h
SOURCES += mainwindow.cpp chartwindow.cpp mdiarea.cpp chartwidget.cpp timeintervalcombobox.cpp
FORMS	=  mainwindow.ui  chartwindow.ui

HEADERS += configurationeditor.h   titleconfigurationeditor.h   timeintervalconfigurationeditor.h
SOURCES += configurationeditor.cpp titleconfigurationeditor.cpp timeintervalconfigurationeditor.cpp
FORMS	+= configurationeditor.ui  titleconfigurationeditor.ui  timeintervalconfigurationeditor.ui

include(logdoc/logdoc.pri)
include(series/series.pri)
