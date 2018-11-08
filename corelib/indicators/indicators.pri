VPATH += $$PWD

HEADERS += bindicator.h indicator.h indicatorfactory.h

HEADERS += blineindicator.h   candleadapterindicator.h   lualineindicator.h
SOURCES += blineindicator.cpp candleadapterindicator.cpp lualineindicator.cpp

HEADERS += lualineindicatorfactory.h
SOURCES += lualineindicatorfactory.cpp

HEADERS += lualineconfigurationeditor.h
SOURCES += lualineconfigurationeditor.cpp
FORMS   += lualineconfigurationeditor.ui
