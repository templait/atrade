TEMPLATE=app
TARGET=datasourcetest
include(../tests.pri)

LIBS += -lcore
INCLUDEPATH += "../../corelib"

QT = core widgets network charts

SOURCES = main.cpp

