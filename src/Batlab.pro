#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T19:05:58
#
#-------------------------------------------------

QT *= core serialport charts multimedia widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BatlabToolkitGUI
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
    dialogs/batlabdebugdialog/batlabdebugdialog.cpp \
    dialogs/newcellplaylistwizard/newcellplaylistwizard.cpp \
    batlabmainwindow.cpp \
    qtautoupdater/autoupdatercore/simplescheduler.cpp \
    qtautoupdater/autoupdatercore/updater.cpp \
    qtautoupdater/autoupdatercore/updater_p.cpp \
    qtautoupdater/autoupdatergui/progressdialog.cpp \
    qtautoupdater/autoupdatergui/updatebutton.cpp \
    qtautoupdater/autoupdatergui/updatecontroller.cpp \
    qtautoupdater/autoupdatergui/updateinfodialog.cpp \
    qtautoupdater/autoupdatergui/dialogmaster.cpp \
    batlabcore/batlabmanager.cpp \
    batlabcore/batlab.cpp \
    components/batlabwidget.cpp \
    batlabcore/batlablogger.cpp \
    batlabcore/batlabcommthread.cpp \
    batlabcore/batlablib.cpp \
    components/cellteststatuswidget.cpp \
    components/resultswidget.cpp \
    batlabcore/cellplaylist.cpp

HEADERS  += \
    dialogs/batlabdebugdialog/batlabdebugdialog.h \
    dialogs/newcellplaylistwizard/newcellplaylistwizard.h \
    batlabmainwindow.h \
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
    qtautoupdater/autoupdatergui/dialogmaster.h \
    batlabcore/version.h \
    batlabcore/batlabmanager.h \
    batlabcore/batlab.h \
    batlabcore/batlablib.h \
    components/batlabwidget.h \
    batlabcore/batlablogger.h \
    components/hexspinbox.h \
    batlabcore/batlabcommthread.h \
    components/cellteststatuswidget.h \
    components/resultswidget.h \
    batlabcore/cellplaylist.h

FORMS    += \
    testwindow.ui \
    cellmodulewidget.ui \
    dialogs/batlabdebugdialog/batlabdebugdialog.ui \
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
