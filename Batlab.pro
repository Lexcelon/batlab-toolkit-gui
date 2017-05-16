#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT       *= core  serialport gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Batlab
TEMPLATE = app

INCLUDEPATH += ./Wizard

SOURCES += main.cpp\
    batlab.cpp \
    batlabconfig.cpp \
    batlabcom.cpp \
    test.cpp \
    batlabcellmanager.cpp \
    batlabcell.cpp \
    batlabsystemtest.cpp \
    testwindow.cpp \
    Wizard/wizardpageone.cpp \
    Wizard/wizardpagetwo.cpp \
    Wizard/wizardpagethree.cpp \
    Wizard/batlabwizard.cpp \
    batlabtestgroup.cpp \
    batlabtestplan.cpp \
    inputStringDialog.cpp

HEADERS  += batlab.h \
    batlabconfig.h \
    batlabcom.h \
    test.h \
    globals.h \
    batlabcellmanager.h \
    batlabcell.h \
    batlabsystemtest.h \
    testwindow.h \
    Wizard/wizardpageone.h \
    Wizard/wizardpagetwo.h \
    Wizard/wizardpagethree.h \
    Wizard/batlabwizard.h \
    batlabtestgroup.h \
    batlabtestplan.h \
    inputStringDialog.h

FORMS    += batlab.ui \
    test.ui \
    testwindow.ui \
    Wizard/wizardpageone.ui \
    Wizard/wizardpagetwo.ui \
    Wizard/wizardpagethree.ui \
    inputStringDialog.ui

DISTFILES += 1010.bmp
