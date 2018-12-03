VPATH += $$PWD

HEADERS += bindicator.h indicator.h indicatorfactory.h

HEADERS += blineindicator.h   indicatorconf.h
SOURCES += blineindicator.cpp indicatorconf.cpp

include(candleadapterindicator/candleadapterindicator.pri)
include(lualineindicator/lualineindicator.pri)
