VPATH += $$PWD

HEADERS += bdatasource.h datasource.h datasourcefactory.h

HEADERS += datasourcequik.h   datasourcefile.h
SOURCES += datasourcequik.cpp datasourcefile.cpp

HEADERS += datasourcefilefactory.h   datasourcequikfactory.h
SOURCES += datasourcefilefactory.cpp datasourcequikfactory.cpp

HEADERS += fileconfigurationeditor.h
SOURCES += fileconfigurationeditor.cpp
FORMS   += fileconfigurationeditor.ui
