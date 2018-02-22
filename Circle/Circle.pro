#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T18:34:05
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Circle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    label.cpp \
    game.cpp

HEADERS  += mainwindow.h \
    label.h \
    game.h

FORMS    += mainwindow.ui

RESOURCES += \
    maps.qrc

CONFIG += MOBILITY
MOBILITY = multimedia
