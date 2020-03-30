QT              += widgets svg
TEMPLATE         = lib
CONFIG          += plugin c++11

TARGET           = shutdown2
DISTFILES       += datetime.json

HEADERS += \
    shutdownplugin.h \
    shutdownwidget.h


SOURCES += \
    shutdownplugin.cpp \
    shutdownwidget.cpp

RESOURCES += resources.qrc
