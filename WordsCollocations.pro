#-------------------------------------------------
#
# Project created by QtCreator 2013-11-24T01:33:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WordsCollocations
TEMPLATE = app

CONFIG += c++11

LIBS += -lcorpus2 -lpwrutils


SOURCES += main.cpp\
        mainwindow.cpp \
    methods.cpp \
    wordsstatisticngrams.cpp \
    methodsngrams.cpp

HEADERS  += mainwindow.h \
    methods.h \
    wordsstatisticngrams.h \
    methodsngrams.h

FORMS    += mainwindow.ui
