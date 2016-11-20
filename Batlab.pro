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
    batlabcom.cpp \
    test.cpp \
    batlabcellmanager.cpp \
    batlabcell.cpp \
    batlabsystemtest.cpp \
    testwindow.cpp \
    wizardpageone.cpp \
    wizardpagetwo.cpp \
    wizardpagethree.cpp \
    batlabwizard.cpp

HEADERS  += batlab.h \
    batlabconfig.h \
    settings.h \
    batlabcom.h \
    test.h \
    globals.h \
    batlabcellmanager.h \
    batlabcell.h \
    batlabsystemtest.h \
    testwindow.h \
    wizardpageone.h \
    wizardpagetwo.h \
    wizardpagethree.h \
    batlabwizard.h

FORMS    += batlab.ui \
    settings.ui \
    test.ui \
    testwindow.ui \
    wizardpageone.ui \
    wizardpagetwo.ui \
    wizardpagethree.ui

DISTFILES += 1010.bmp
