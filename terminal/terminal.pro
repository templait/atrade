TEMPLATE=app
TARGET=terminal
include(../project.pri)

QT = core widgets network charts

SOURCES = main.cpp

HEADERS += bdatasource.h   log.h   etimeinterval.h
SOURCES += bdatasource.cpp log.cpp

HEADERS += mainwindow.h   chartwindow.h   chartwidget.h   candlestickseries.h   candle.h   datasourcequik.h
SOURCES += mainwindow.cpp chartwindow.cpp chartwidget.cpp candlestickseries.cpp candle.cpp datasourcequik.cpp
FORMS	=  mainwindow.ui

include(logdoc/logdoc.pri)
