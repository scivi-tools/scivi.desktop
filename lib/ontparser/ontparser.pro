#-------------------------------------------------
#
# Project created by QtCreator 2018-01-25T18:09:29
#
#-------------------------------------------------

QT       -= gui

TARGET = ontparser
TEMPLATE = lib
CONFIG += staticlib
CONFIG -= debug_and_release_target

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ont/node.cpp \
    ont/ontology.cpp \
    ont/relation.cpp \
    ont/ontologydata.cpp

HEADERS += \
    ont/node.h \
    ont/ontology.h \
    ont/relation.h \
    ont/ontologydata.h
