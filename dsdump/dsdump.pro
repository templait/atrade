TEMPLATE=app
TARGET=dsdump
include(../project.pri)

CONFIG += console

QT = core network charts

LIBS += -lcore
INCLUDEPATH += ../corelib

SOURCES = main.cpp
