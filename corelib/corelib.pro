TEMPLATE=lib
TARGET=core
include(../project.pri)

QT = core network charts

HEADERS = types.h serial.hpp sharedpointer.hpp factory.hpp

HEADERS += log.h   tools.h   candle.h   point.h
SOURCES += log.cpp tools.cpp candle.cpp point.cpp

include(datasources/datasources.pri)
include(indicators/indicators.pri)

LIBS += -llua
