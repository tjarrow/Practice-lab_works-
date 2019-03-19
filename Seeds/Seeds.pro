#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T01:30:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Seeds
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib

LIBS += -lopencv_core \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_objdetect

SOURCES += main.cpp\
        mainwindow.cpp \
    watershed.cpp \
    wheel.cpp

HEADERS  += mainwindow.h \
    watershed.h \
    wheel.h

FORMS    += mainwindow.ui
