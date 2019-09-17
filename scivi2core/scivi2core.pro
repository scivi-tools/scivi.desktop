#-------------------------------------------------
#
# Project created by QtCreator 2017-11-26T17:28:52
#
#-------------------------------------------------

QT -= gui
QT       += quick
CONFIG += c++14 staticlib
CONFIG -= debug_and_release_target

INCLUDEPATH += src/
TARGET = scivi2core
TEMPLATE = lib

DEFINES += SCIVI_LIBRARY

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/combineddatanode.cpp \
    src/dataflowdiagram.cpp \
    src/datanode.cpp \
    src/edge.cpp \
    src/edgeview.cpp \
    src/filterimplementation.cpp \
    src/node.cpp \
    src/nodegroup.cpp \
    src/nodesocket.cpp \
    src/nodeview.cpp \
    src/setting.cpp \
    src/settingsmodel.cpp \
    src/sinkfilter.cpp \
    src/graphview.cpp \
    src/utils/error.cpp

HEADERS += \
    src/combineddatanode.h \
    src/dataflowdiagram.h \
    src/datanode.h \
    src/edge.h \
    src/edgeview.h \
    src/filterfactory.h \
    src/filterimplementation.h \
    src/node.h \
    src/nodegroup.h \
    src/nodesocket.h \
    src/nodeview.h \
    src/plugin.h \
    src/scivi2lib_global.h \
    src/setting.h \
    src/settingsmodel.h \
    src/sinkfilter.h \
    src/utils/path.h \
    src/visual.h \
    src/graphview.h \
    src/graphview.hpp \
    src/utils/error.h

DESTDIR = $$PWD/lib
