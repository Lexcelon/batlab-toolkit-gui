#ifndef BATLAB_MAIN_WINDOW_H
#define BATLAB_MAIN_WINDOW_H

#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QSpinBox>
#include <QCloseEvent>

#include <stdio.h>
#include <cstdio>
#include <iostream>

#include "batlabconfig.h"
#include "settings.h"
#include "dialogs/batlabdebugdialog/batlabdebugdialog.h"
#include "batlabcore/batlab.h"
#include "batlabcellmanager.h"
#include "batlabcore/version.h"
#include "batlabcore/batlabmanager.h"
#include "batlabcore/batlablogger.h"
#include "components/batlabwidget.h"
#include "dialogs/newcellplaylistwizard/newcellplaylistwizard.h"
#include "testdata.h"
#include "qtautoupdater/autoupdatergui/updatebutton.h"
#include "qtautoupdater/autoupdatergui/updatecontroller.h"

namespace Ui {
class MainWindow;
}

class BatlabMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BatlabMainWindow(QWidget *parent = 0);
    ~BatlabMainWindow();

public slots:
    void updateLiveViewWithWriteCommand(int serialNumber, int nameSpace,int batlabRegister, int value);
    void updateLiveViewWithReadCommand(int serialNumber, int nameSpace,int batlabRegister);
    void updateLiveViewWithWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void updateLiveViewWithReadResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void updateLiveViewWithReceivedStream(int cell, int mode, int status, float temp, float current, float voltage);

    void updateLiveViewTextBrowser(QString);

    void redrawBatlabInfo(QVector<batlabInfo> infos);

    void togglePrintDebugMessages(bool value);

signals:
    void emitUpdateText(QString);

private:
    void closeEvent(QCloseEvent *event);

    BatlabManager *batlabManager;

    BatlabDebugDialog *batlabDebugDialog = nullptr;
    batlabCellManager * cellManager = nullptr;

    void initializeMainWindowUI();
    void createMenus();
    void createActions();

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newCellPlaylistAct;
    QAction *openCellPlaylistAct;
    QAction *exitBatlabToolkitGUIAct;

    QAction *debugBatlabAct;
    QAction *checkForBatlabFirmwareUpdatesAct;

    QAction *aboutBatlabToolkitGUIAct;
    QAction *checkForUpdatesAct;

    QtAutoUpdater::UpdateController *applicationUpdateController;

    QDialogButtonBox *tabButtonBox;
    QPushButton *cellPlaylistButton;
    QPushButton *batlabsButton;
    QPushButton *liveViewButton;
    QPushButton *resultsButton;

    QWidget *centralWidget;
    QGridLayout *centralWidgetLayout;

    QTabWidget *mainTabWidget;
    QWidget *testCellsTab;
    QGridLayout *testCellsTabLayout;
    QWidget *configurePackTab;
    QHBoxLayout *configurePackTabLayout;

    QStackedWidget *mainStackedWidget;
    QFrame *cellPlaylistTabWidget;
    QFrame *batlabsTabWidget;
    QFrame *liveViewTabWidget;
    QFrame *resultsTabWidget;

    QWidget *cellPlaylistNotLoadedWidget;
    QWidget *cellPlaylistLoadedWidget;
    QGridLayout *cellPlaylistNotLoadedLayout;
    QGridLayout *cellPlaylistLoadedLayout;
    QLabel *noCellPlaylistLoadedLabel;
    QPushButton *newCellPlaylistButton;
    QPushButton *openCellPlaylistButton;
    QStackedWidget *cellPlaylistStackedWidget;

    QHBoxLayout *liveViewButtonLayout;
    QCheckBox *liveViewPrintDebugCheckBox;
    QPushButton *liveViewClearButton;
    QPushButton *liveViewSaveButton;
    bool printDebugMessages;

    QVBoxLayout *batlabsTabLayout;
    QGridLayout *cellPlaylistTabLayout;
    QVBoxLayout *liveViewTabLayout;

    QTextBrowser *liveViewTextBrowser;

private slots:
    void newCellPlaylist();
    void openCellPlaylist();
    void exitBatlabToolkitGUI();

    void debugBatlab();
    void checkForBatlabFirmwareUpdates();

    void aboutBatlabToolkitGUI();

    void showNewCellPlaylistWizard();
    void loadPlaylistIntoGUI();

    void saveLiveView();
    void processQtLogMessage(QtMsgType type, QString str);

    void processRegisterReadRequest(int serial, int ns, int address);
    void processRegisterWriteRequest(int serial, int ns, int address, int value);

};

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // BATLAB_MAIN_WINDOW_H
