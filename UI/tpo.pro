#-------------------------------------------------
#
# Project created by QtCreator 2014-11-20T19:56:57
#
#-------------------------------------------------
include(qextserialport-1.2rc/src/qextserialport.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tpo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
