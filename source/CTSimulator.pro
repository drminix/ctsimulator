#-------------------------------------------------
#
# Project created by QtCreator 2012-07-22T14:39:52
#
#-------------------------------------------------

QT       += core gui
QT += opengl


TARGET = CTSimulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp \
    drawingarea.cpp \
    utility.cpp \
    matrixwidget.cpp \
    sinogramwidget.cpp \
    artthread.cpp \
    imagewidget.cpp \
    sirtthread.cpp

HEADERS  += mainwindow.h \
    matrix.h \
    drawingarea.h \
    utility.h \
    matrixwidget.h \
    sinogramwidget.h \
    artthread.h \
    imagewidget.h \
    sirtthread.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=gnu++0x
