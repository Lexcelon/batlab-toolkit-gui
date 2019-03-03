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

#include "dialogs/batlabdebugdialog/batlabdebugdialog.h"
#include "batlabcore/batlab.h"
#include "batlabcore/version.h"
#include "batlabcore/batlabmanager.h"
#include "batlabcore/batlablogger.h"
#include "components/batlabwidget.h"
#include "components/resultswidget.h"
#include "components/playlistsettingswidget.h"
#include "dialogs/newcellplaylistwizard/newcellplaylistwizard.h"
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
    void updatelogViewWithWriteCommand(int serialNumber, int nameSpace,int batlabRegister, int value);
    void updatelogViewWithReadCommand(int serialNumber, int nameSpace,int batlabRegister);
    void updatelogViewWithWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void updatelogViewWithReadResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void updatelogViewWithReceivedStream(int cell, int mode, int status, float temp, float current, float voltage);

    void updatelogViewTextBrowser(QString);

    void redrawBatlabInfo(QVector<batlabStatusInfo> infos, int latestFirmwareVersion);
    void redrawResultsInfo(QVector<cellResultsStatusInfo> infos);

    void togglePrintDebugMessages(bool value);

signals:
    void emitUpdateText(QString);

private:
    void closeEvent(QCloseEvent *event);

    BatlabManager *batlabManager;
    PlaylistSettingsWidget *playlistSettingsWidget;

    BatlabDebugDialog *batlabDebugDialog = nullptr;

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

    QAction *userGuideAct;
    QAction *aboutBatlabToolkitGUIAct;
    QAction *checkForUpdatesAct;

    QtAutoUpdater::UpdateController *applicationUpdateController;

    QVBoxLayout *tabButtonBox;
    QPushButton *cellPlaylistButton;
    QPushButton *batlabsButton;
    QPushButton *logViewButton;
    QPushButton *resultsButton;
    QPushButton *startTestsButton;
    QPushButton *stopTestsButton;

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
    QFrame *logViewTabWidget;
    QFrame *resultsTabWidget;

    QWidget *cellPlaylistNotLoadedWidget;
    PlaylistSettingsWidget *cellPlaylistLoadedWidget;
    QGridLayout *cellPlaylistNotLoadedLayout;
    QLabel *noCellPlaylistLoadedLabel;
    QPushButton *newCellPlaylistButton;
    QPushButton *openCellPlaylistButton;
    QStackedWidget *cellPlaylistStackedWidget;

    QHBoxLayout *logViewButtonLayout;
    QCheckBox *logViewPrintDebugCheckBox;
    QPushButton *logViewClearButton;
    QPushButton *logViewSaveButton;
    bool printDebugMessages;

    QVBoxLayout *batlabsTabLayout;
    QVBoxLayout *resultsTabLayout;
    QGridLayout *cellPlaylistTabLayout;
    QVBoxLayout *logViewTabLayout;

    QTextBrowser *logViewTextBrowser;

private slots:
    void openCellPlaylist();
    void exitBatlabToolkitGUI();

    void debugBatlab();
    void checkForBatlabFirmwareUpdates();

    void aboutBatlabToolkitGUI();
    void openUserGuide();

    void showNewCellPlaylistWizard();
    void displayLoadedCellPlaylist(CellPlaylist playlist);

    void savelogView();
    void processQtLogMessage(QtMsgType type, QString str);

    void processRegisterReadRequest(int serial, int ns, int address);
    void processRegisterWriteRequest(int serial, int ns, int address, int value);
    void processFirmwareFlashRequest(int serial, QString firmwareVersion);

};

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // BATLAB_MAIN_WINDOW_H
