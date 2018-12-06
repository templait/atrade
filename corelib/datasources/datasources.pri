VPATH += $$PWD

HEADERS += bdatasource.h datasource.h datasourcefactory.h datasourceconf.h

HEADERS += datasourceconfhelper.h
SOURCES += datasourceconfhelper.cpp

include(filedatasource/filedatasource.pri)
include(quikdatasource/quikdatasource.pri)
