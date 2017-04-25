#-------------------------------------------------
#
# Project created by QtCreator 2017-04-25T10:47:24
#
#-------------------------------------------------

QT       += core gui
QT += axcontainer
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cutSerialPortMsg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ExcelBase.cpp \
    checkout.cpp \
    feedbackserial.cpp \
    readserialdata.cpp

HEADERS  += mainwindow.h \
    ExcelBase.h \
    checkout.h \
    feedbackserial.h \
    readserialdata.h \
    data.h

FORMS    += mainwindow.ui

DISTFILES +=
