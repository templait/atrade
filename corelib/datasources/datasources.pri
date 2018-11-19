VPATH += $$PWD

HEADERS += bdatasource.h datasource.h datasourcefactory.h

HEADERS += datasourcequik.h
SOURCES += datasourcequik.cpp

HEADERS += datasourcequikfactory.h
SOURCES += datasourcequikfactory.cpp

include(filedatasource/filedatasource.pri)
