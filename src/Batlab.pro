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

win32 {
    QT += winextras
    LIBS += -lAdvapi32 -lOle32 -lShell32
    SOURCES += qtautoupdater/autoupdatergui/adminauthorization_win.cpp
} else:mac {
    LIBS += -framework Security
    SOURCES += qtautoupdater/autoupdatergui/adminauthorization_mac.cpp
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
    testdata.cpp \
    qtautoupdater/autoupdatercore/simplescheduler.cpp \
    qtautoupdater/autoupdatercore/updater.cpp \
    qtautoupdater/autoupdatercore/updater_p.cpp \
    qtautoupdater/autoupdatergui/progressdialog.cpp \
    qtautoupdater/autoupdatergui/updatebutton.cpp \
    qtautoupdater/autoupdatergui/updatecontroller.cpp \
    qtautoupdater/autoupdatergui/updateinfodialog.cpp \
    settings.cpp \
    qtautoupdater/autoupdatergui/dialogmaster.cpp \
    qtautoupdater/winextras/qwinevent.cpp \
    qtautoupdater/winextras/qwineventfilter.cpp \
    qtautoupdater/winextras/qwinfunctions.cpp \
    qtautoupdater/winextras/qwinfunctions_p.cpp \
    qtautoupdater/winextras/qwinjumplist.cpp \
    qtautoupdater/winextras/qwinjumplistcategory.cpp \
    qtautoupdater/winextras/qwinjumplistitem.cpp \
    qtautoupdater/winextras/qwinmime.cpp \
    qtautoupdater/winextras/qwintaskbarbutton.cpp \
    qtautoupdater/winextras/qwintaskbarprogress.cpp \
    qtautoupdater/winextras/qwinthumbnailtoolbar.cpp \
    qtautoupdater/winextras/qwinthumbnailtoolbutton.cpp \
    qtautoupdater/winextras/windowsguidsdefs.cpp

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
    Wizard/newtestschedulewizard.h \
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
    version.h \
    qtautoupdater/winextras/qwinevent.h \
    qtautoupdater/winextras/qwineventfilter_p.h \
    qtautoupdater/winextras/qwinextrasglobal.h \
    qtautoupdater/winextras/qwinfunctions.h \
    qtautoupdater/winextras/qwinfunctions_p.h \
    qtautoupdater/winextras/qwinjumplist.h \
    qtautoupdater/winextras/qwinjumplistcategory.h \
    qtautoupdater/winextras/qwinjumplistcategory_p.h \
    qtautoupdater/winextras/qwinjumplistitem.h \
    qtautoupdater/winextras/qwinjumplistitem_p.h \
    qtautoupdater/winextras/qwinjumplist_p.h \
    qtautoupdater/winextras/qwinmime.h \
    qtautoupdater/winextras/qwintaskbarbutton.h \
    qtautoupdater/winextras/qwintaskbarbutton_p.h \
    qtautoupdater/winextras/qwintaskbarprogress.h \
    qtautoupdater/winextras/qwinthumbnailtoolbar.h \
    qtautoupdater/winextras/qwinthumbnailtoolbar_p.h \
    qtautoupdater/winextras/qwinthumbnailtoolbutton.h \
    qtautoupdater/winextras/qwinthumbnailtoolbutton_p.h \
    qtautoupdater/winextras/windowsguidsdefs_p.h \
    qtautoupdater/winextras/winpropkey_p.h \
    qtautoupdater/winextras/winshobjidl_p.h \
    qtautoupdater/winextras/qplatformnativeinterface.h

FORMS    += \
    test.ui \
    testwindow.ui \
    Wizard/wizardpageone.ui \
    Wizard/wizardpagetwo.ui \
    Wizard/wizardpagethree.ui \
    cellmodulewidget.ui \
    inputstringdialog.ui \
    mainwindow.ui \
    qtautoupdater/autoupdatergui/progressdialog.ui \
    qtautoupdater/autoupdatergui/updatebutton.ui \
    qtautoupdater/autoupdatergui/updateinfodialog.ui \
    settings.ui

DISTFILES += 1010.bmp \
    qtautoupdater/autoupdatergui/translations/translations.pri \
    qtautoupdater/autoupdatergui/update-loader.gif \
    qtautoupdater/autoupdatergui/update.ico \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_de.ts \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_es.ts \
    qtautoupdater/autoupdatergui/translations/qtautoupdatergui_template.ts

DEPENDPATH  *= $$DESTDIR

RESOURCES += \
    qtautoupdater/autoupdatergui/autoupdatergui_resource.qrc
