TEMPLATE=lib
TARGET=core
include(../project.pri)

QT = core network charts

HEADERS = types.h serial.hpp sharedpointer.hpp factory.hpp

HEADERS += log.h   tools.h   candle.h   point.h   configuration.h   productconfigurationeditor.h   productcombobox.h
SOURCES += log.cpp tools.cpp candle.cpp point.cpp configuration.cpp productconfigurationeditor.cpp productcombobox.cpp

HEADERS += productlistmodel.h   configurationmodel.h
SOURCES += productlistmodel.cpp configurationmodel.cpp

include(datasources/datasources.pri)
include(indicators/indicators.pri)

LIBS += -llua
