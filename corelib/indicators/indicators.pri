VPATH += $$PWD

HEADERS += bindicator.h indicator.h indicatorfactory.h

HEADERS += blineindicator.h   lualineindicator.h
SOURCES += blineindicator.cpp lualineindicator.cpp

HEADERS += lualineindicatorfactory.h
SOURCES += lualineindicatorfactory.cpp

HEADERS += lualineconfigurationeditor.h
SOURCES += lualineconfigurationeditor.cpp
FORMS   += lualineconfigurationeditor.ui

include(candleadapterindicator/candleadapterindicator.pri)
