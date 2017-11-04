#ifndef BATLAB_H
#define BATLAB_H

#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QSpinBox>
#include <QCloseEvent>

#include "batlabconfig.h"
#include "settings.h"
#include "test.h"
#include "batlabcom.h"
#include "batlabcellmanager.h"
#include "version.h"
#include "wizardpageone.h"
#include "wizardpagetwo.h"
#include "newtestschedulewizard.h"
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
    //void onConnectToBatlabs(QStringList names);
    void removeBatlabConnection(QString batlabUnitPortName);
    //void onGetBatlabNames();
    void onTest();

    void onReceiveWriteCommand(int serialNumber, int nameSpace,int batlabRegister, int value);
    void onReceiveReadCommand(int serialNumber, int nameSpace,int batlabRegister);
    void onReceiveWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void onReceiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void onReceiveStream(int cell, int mode, int status, float temp, float current, float voltage);

    void onNewCellPlaylistWizard();
    void onNewProjectWizard();
    void onLoadTest(QString);
    void onLoadProject();

    void onTestDataButton();
    void onProcessPack();
    void onPackBuilt(QVector<QStringList> list);

    void onFinishedTests(QString designator, int testNum);

    void onUpdateText(QString);

signals:
    void emitUpdateText(QString);
protected:
    void closeEvent(QCloseEvent *event)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Exit Batlab", "Are you sure you want to quit?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            for (int i = 0; i < batlabComObjects.size(); ++i) {
                batlabComObjects[i]->setAllIdle();
            }
            event->accept();
        } else {
            event->ignore();
        }
    }

private:
    Ui::MainWindow *ui = nullptr;
    QPushButton *connectToBatlabs = nullptr;
    QPushButton *test = nullptr;
    QVector<batlabCom*> batlabComObjects;

    batlabtest *testObj = nullptr;
    batlabCellManager * cellManager = nullptr;

    QTableWidget * tableWidget = nullptr;

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

    QtAutoUpdater::UpdateController *updaterController;

private slots:
    void newCellPlaylist();
    void openCellPlaylist();
    void exitBatlabToolkitGUI();

    void debugBatlab();
    void checkForBatlabFirmwareUpdates();

    void aboutBatlabToolkitGUI();

    void makeBatlabConnections(QStringList availCommPortNames);
    void updateBatlabConnections();

    QStringList getAvailCommPortNames();

};

#endif // BATLAB_H
