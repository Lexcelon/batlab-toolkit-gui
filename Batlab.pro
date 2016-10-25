#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT       += core gui multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Batlab
TEMPLATE = app


SOURCES += main.cpp\
    batlab.cpp \
    batlabconfig.cpp \
    settings.cpp \
    bldataobject.cpp \
    batlabpacket.cpp \
    batlabcom.cpp \
    test.cpp

HEADERS  += batlab.h \
    batlabconfig.h \
    settings.h \
    bldataobject.h \
    batlabpacket.h \
    batlabcom.h \
    test.h \
    globals.h

FORMS    += batlab.ui \
    settings.ui \
    test.ui

DISTFILES += 1010.bmp
