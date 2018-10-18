TEMPLATE=lib
TARGET=core
include(../project.pri)

QT = core network charts

HEADERS = types.h serial.hpp

HEADERS += log.h   candle.h
SOURCES += log.cpp candle.cpp

include(datasources/datasources.pri)
include(indicators/indicators.pri)
