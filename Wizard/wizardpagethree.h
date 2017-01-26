#ifndef WIZARDPAGETHREE_H
#define WIZARDPAGETHREE_H

#include <QWidget>
#include "../globals.h"
#include <QLabel>
#include <QFile>
#include <QProgressDialog>

namespace Ui {
class wizardPageThree;
}

class wizardPageThree : public QWidget
{
    Q_OBJECT

public:
    explicit wizardPageThree(QWidget *parent = 0);
    ~wizardPageThree();

public slots:
    void onDesignator(QString);
    void onStartInd(QString);
    void onNumCells(int);
    void onTestParms(testParms);
    void onCCR(double);
    void onDCR(double);
    void onCapacity(double);
    void onRestTime(int);
    void onActivate();
    void onNumCycles(int);
    QString onGetName(int);
    void onSaveProject();
    void onProjectName(QString);
    QString onGetProjectName() { return projectName; }

private:
    Ui::wizardPageThree *ui;

    QString designator;
    QString startInd;
    int numCells;
    int numCycles;
    testParms parms;
    QString projectName;

    double dischargeCurrentCRate;
    double chargeCurrentCRate;
    double capacity;
    int restTime;
};

#endif // WIZARDPAGETHREE_H
