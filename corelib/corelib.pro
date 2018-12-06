TEMPLATE=lib
TARGET=core
include(../project.pri)

QT = core network charts

HEADERS = types.h serial.hpp sharedpointer.hpp factory.hpp

HEADERS += parentconf.hpp addchildparentconf.hpp productconfhelper.hpp

HEADERS += bconf.h   timeintervalconf.h   productconf.h
SOURCES += bconf.cpp timeintervalconf.cpp productconf.cpp

HEADERS += log.h   tools.h   candle.h   point.h   confeditormodule.h   productcombobox.h
SOURCES += log.cpp tools.cpp candle.cpp point.cpp confeditormodule.cpp productcombobox.cpp

HEADERS += productlistmodel.h
SOURCES += productlistmodel.cpp

include(datasources/datasources.pri)
include(indicators/indicators.pri)

LIBS += -llua
