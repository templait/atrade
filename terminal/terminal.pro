TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp

HEADERS += chartwindowconf.h   chartconf.h
SOURCES += chartwindowconf.cpp chartconf.cpp

HEADERS += mainwindow.h   chartwindow.h   mdiarea.h   chartwidget.h   timeintervalcombobox.h
SOURCES += mainwindow.cpp chartwindow.cpp mdiarea.cpp chartwidget.cpp timeintervalcombobox.cpp
FORMS	=  mainwindow.ui  chartwindow.ui

HEADERS += confeditor.h   titleconfeditor.h   timeintervalconfeditor.h
SOURCES += confeditor.cpp titleconfeditor.cpp timeintervalconfeditor.cpp
FORMS	+= confeditor.ui  titleconfeditor.ui  timeintervalconfeditor.ui

include(logdoc/logdoc.pri)
include(productdoc/productdoc.pri)
include(series/series.pri)
