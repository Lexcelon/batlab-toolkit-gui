#ifndef BATLAB_H
#define BATLAB_H

#include <QMainWindow>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsView>
#include "batlabconfig.h"
#include "settings.h"
#include "test.h"
#include "batlabcom.h"
#include "batlabcellmanager.h"
#include "globals.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWizard>
#include <QWizardPage>
#include <QLineEdit>
#include <QSpinBox>
#include "wizardpageone.h"
#include "wizardpagetwo.h"
#include "batlabwizard.h"

namespace Ui {
class Batlab;
}

class Batlab : public QMainWindow
{
    Q_OBJECT

public:
    explicit Batlab(QWidget *parent = 0);
    ~Batlab();

public slots:
    void onConnectToBatlabs(QStringList names);
    void onGetBatlabNames();
    void onTest();

    void onReceiveWriteCommand(int serialNumber, int nameSpace,int batlabRegister, int value);
    void onReceiveReadCommand(int serialNumber, int nameSpace,int batlabRegister);
    void onReceiveWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void onReceiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb);
    void onReceiveStream(int cell, int mode, int status, float temp, float current, float voltage);

    void onNewProjectWizard();
    void onAddTests();
    void onLoadTest(QString);
    void onLoadProject();

    void onFinishedTests(QString designator, int testNum);

private:
    Ui::Batlab *ui = nullptr;
    QPushButton *connectToBatlabs = nullptr;
    QPushButton *test = nullptr;
    QPushButton *loadProject = nullptr;
    QPushButton *newProjectWizard = nullptr;
    QPushButton *report = nullptr;
    QPushButton *options = nullptr;
    QPushButton *exit = nullptr;
    QGraphicsView *glWidget = nullptr;
    batlabtest *testObj = nullptr;
    QVector<batlabCom*> batlabComObjects;

    settings * configSettings = nullptr;

    batlabCellManager * cellManager = nullptr;

    QTableWidget * tableWidget = nullptr;

};

#endif // BATLAB_H
