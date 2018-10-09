TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

SOURCES = main.cpp

HEADERS += log.h   etimeinterval.h
SOURCES += log.cpp

HEADERS += mainwindow.h   chartwindow.h   chartwidget.h   candle.h
SOURCES += mainwindow.cpp chartwindow.cpp chartwidget.cpp candle.cpp
FORMS	=  mainwindow.ui

include(logdoc/logdoc.pri)
include(datasources/datasources.pri)
include(series/series.pri)