#-------------------------------------------------
#
# Project created by QtCreator 2013-11-24T01:33:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WordsCollocations
TEMPLATE = app


LIBS += -lcorpus2 -lpwrutils


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
