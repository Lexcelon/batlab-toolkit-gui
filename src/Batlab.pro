#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT *=  core serialport charts multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Batlab
TEMPLATE = app

INCLUDEPATH += ./Wizard

@
CONFIG += debug_and_release
@

SOURCES += main.cpp\
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
    batlabtestgroup.cpp \
    batlabtestplan.cpp \
    inputStringDialog.cpp \
    HistogramWidget.cpp \
    HistogramObject.cpp \
    TestData.cpp \
    CellModuleWidget.cpp \
    Wizard/newTestScheduleWizard.cpp \
    BatlabMainWindow.cpp \
    dialogs/newcellplaylistwizard/newcellplaylistwizard.cpp \
    globals.cpp

HEADERS  += \
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
    batlabtestgroup.h \
    batlabtestplan.h \
    inputStringDialog.h \
    HistogramWidget.h \
    HistogramObject.h \
    TestData.h \
    CellModuleWidget.h \
    Wizard/newTestScheduleWizard.h \
    BatlabMainWindow.h \
    GUIversion.h \
    dialogs/newcellplaylistwizard/newcellplaylistwizard.h

FORMS    += \
    test.ui \
    testwindow.ui \
    Wizard/wizardpageone.ui \
    Wizard/wizardpagetwo.ui \
    Wizard/wizardpagethree.ui \
    inputStringDialog.ui \
    CellModuleWidget.ui \
    MainWindow.ui

DISTFILES += 1010.bmp

DEPENDPATH  *= $$DESTDIR
