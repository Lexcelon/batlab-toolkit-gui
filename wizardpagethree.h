#ifndef WIZARDPAGETHREE_H
#define WIZARDPAGETHREE_H

#include <QWidget>
#include "globals.h"
#include <QLabel>
#include <QFile>

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
    void onActivate();
    void onNumCycles(int);
    QString onGetName(int);
    void onSaveProject();

private:
    Ui::wizardPageThree *ui;

    QString designator;
    QString startInd;
    int numCells;
    int numCycles;
    testParms parms;
};

#endif // WIZARDPAGETHREE_H
