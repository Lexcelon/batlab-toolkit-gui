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
    dialogs/newcellplaylistwizard/newcellplaylistwizard.cpp \
    globals.cpp \
    Wizard/newtestschedulewizard.cpp \
    batlabmainwindow.cpp \
    cellmodulewidget.cpp \
    histogramobject.cpp \
    histogramwidget.cpp \
    inputstringdialog.cpp \
    testdata.cpp

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
    dialogs/newcellplaylistwizard/newcellplaylistwizard.h \
    version.h \
    testdata.h \
    histogramwidget.h \
    inputstringdialog.h \
    batlabmainwindow.h \
    cellmodulewidget.h \
    histogramobject.h \
    Wizard/newtestschedulewizard.h

FORMS    += \
    test.ui \
    testwindow.ui \
    Wizard/wizardpageone.ui \
    Wizard/wizardpagetwo.ui \
    Wizard/wizardpagethree.ui \
    cellmodulewidget.ui \
    inputstringdialog.ui \
    mainwindow.ui

DISTFILES += 1010.bmp

DEPENDPATH  *= $$DESTDIR
