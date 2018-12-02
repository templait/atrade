VPATH += $$PWD

HEADERS += bindicator.h indicator.h indicatorfactory.h indicatorconf.h

HEADERS += blineindicator.h
SOURCES += blineindicator.cpp

include(candleadapterindicator/candleadapterindicator.pri)
include(lualineindicator/lualineindicator.pri)
