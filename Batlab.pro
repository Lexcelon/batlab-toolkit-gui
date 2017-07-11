#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT       *= core  serialport charts multimedia widgets

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
    inputStringDialog.cpp \
    HistogramWidget.cpp \
    HistogramObject.cpp \
    TestData.cpp \
    CellModuleWidget.cpp

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
    inputStringDialog.h \
    HistogramWidget.h \
    HistogramObject.h \
    TestData.h \
    CellModuleWidget.h

FORMS    += batlab.ui \
    test.ui \
    testwindow.ui \
    Wizard/wizardpageone.ui \
    Wizard/wizardpagetwo.ui \
    Wizard/wizardpagethree.ui \
    inputStringDialog.ui \
    CellModuleWidget.ui

DISTFILES += 1010.bmp

DEPENDPATH  *= $$DESTDIR
