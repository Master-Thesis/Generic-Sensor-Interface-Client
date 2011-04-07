#-------------------------------------------------
#
# Project created by QtCreator 2011-02-20T19:15:42
#
#-------------------------------------------------

QT       += \
            network

TARGET = ThesisNetworkClient
CONFIG   += app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    udpclient.cpp \
    configwidget.cpp \
    node.cpp \
    nodesocket.cpp \
    configWidgetPages/networkpage.cpp \
    configWidgetPages/settingspage.cpp \
    configWidgetPages/consolepage.cpp \
    configWidgetPages/gamepage.cpp \
    tetrixWidget/tetrixwindow.cpp \
    tetrixWidget/tetrixpiece.cpp \
    tetrixWidget/tetrixboard.cpp \
    configfilehandler.cpp

HEADERS += \
    udpclient.h \
    configwidget.h \
    node.h \
    nodesocket.h \
    configWidgetPages/networkpage.h \
    configWidgetPages/settingspage.h \
    configWidgetPages/consolepage.h \
    configWidgetPages/ip4validator.h \
    configWidgetPages/gamepage.h \
    tetrixWidget/tetrixwindow.h \
    tetrixWidget/tetrixpiece.h \
    tetrixWidget/tetrixboard.h \
    configfilehandler.h

RESOURCES += \
    configpages.qrc
