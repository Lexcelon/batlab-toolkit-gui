#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT *=  core serialport charts multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Batlab
TEMPLATE = app

@
CONFIG += debug_and_release
@

win32 {
    QT += winextras
    LIBS += -lAdvapi32 -lOle32 -lShell32
    SOURCES += qtautoupdater/autoupdatergui/adminauthorization_win.cpp
    RC_ICONS += icons/desktop-favicon.ico
} else:mac {
    LIBS += -framework Security
    SOURCES += qtautoupdater/autoupdatergui/adminauthorization_mac.cpp
    ICON = icons/desktop-favicon.icns
} else:unix {
    LIBS += -lutil
    SOURCES += qtautoupdater/autoupdatergui/adminauthorization_x11.cpp
}

SOURCES += main.cpp\
    batlabconfig.cpp \
    batlabcom.cpp \
    test.cpp \
    batlabcellmanager.cpp \
    batlabcell.cpp \
    batlabsystemtest.cpp \
    testwindow.cpp \
    batlabtestgroup.cpp \
    batlabtestplan.cpp \
    dialogs/newcellplaylistwizard/newcellplaylistwizard.cpp \
    globals.cpp \
    batlabmainwindow.cpp \
    cellmodulewidget.cpp \
    histogramobject.cpp \
    histogramwidget.cpp \
    inputstringdialog.cpp \
    testdata.cpp \
    qtautoupdater/autoupdatercore/simplescheduler.cpp \
    qtautoupdater/autoupdatercore/updater.cpp \
    qtautoupdater/autoupdatercore/updater_p.cpp \
    qtautoupdater/autoupdatergui/progressdialog.cpp \
    qtautoupdater/autoupdatergui/updatebutton.cpp \
    qtautoupdater/autoupdatergui/updatecontroller.cpp \
    qtautoupdater/autoupdatergui/updateinfodialog.cpp \
    settings.cpp \
    qtautoupdater/autoupdatergui/dialogmaster.cpp

HEADERS  += \
    batlabconfig.h \
    batlabcom.h \
    test.h \
    globals.h \
    batlabcellmanager.h \
    batlabcell.h \
    batlabsystemtest.h \
    testwindow.h \
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
    qtautoupdater/autoupdatercore/adminauthoriser.h \
    qtautoupdater/autoupdatercore/qtautoupdatercore_global.h \
    qtautoupdater/autoupdatercore/simplescheduler_p.h \
    qtautoupdater/autoupdatercore/updater.h \
    qtautoupdater/autoupdatercore/updater_p.h \
    qtautoupdater/autoupdatergui/adminauthorization_p.h \
    qtautoupdater/autoupdatergui/progressdialog_p.h \
    qtautoupdater/autoupdatergui/qtautoupdatergui_global.h \
    qtautoupdater/autoupdatergui/updatebutton.h \
    qtautoupdater/autoupdatergui/updatebutton_p.h \
    qtautoupdater/autoupdatergui/updatecontroller.h \
    qtautoupdater/autoupdatergui/updatecontroller_p.h \
    qtautoupdater/autoupdatergui/updateinfodialog_p.h \
    settings.h \
    qtautoupdater/autoupdatergui/dialogmaster.h \
    version.h

FORMS    += \
    test.ui \
    testwindow.ui \
    cellmodulewidget.ui \
    inputstringdialog.ui \
    qtautoupdater/autoupdatergui/progressdialog.ui \
    qtautoupdater/autoupdatergui/updatebutton.ui \
    qtautoupdater/autoupdatergui/updateinfodialog.ui \
    settings.ui

DISTFILES += \
    qtautoupdater/autoupdatergui/translations/translations.pri \
    qtautoupdater/autoupdatergui/update-loader.gif \
    qtautoupdater/autoupdatergui/update.ico \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_de.ts \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_es.ts \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_template.ts

DEPENDPATH  *= $$DESTDIR

RESOURCES += \
    qtautoupdater/autoupdatergui/autoupdatergui_resource.qrc
