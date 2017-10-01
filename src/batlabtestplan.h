#ifndef BATLABTESTPLAN_H
#define BATLABTESTPLAN_H

#include <QObject>
#include "globals.h"
#include "batlabcell.h"
#include "batlabtestgroup.h"
#include <QMessageBox>

class batlabTestPlan : public QObject
{
    Q_OBJECT

public:
    batlabTestPlan();
    batlabTestPlan(int numBatlabs, QVector<batlabCell*> list);
    batlabTestPlan(int numBatlabs,QVector<batlabCell*> list, QVector<batlabCom*> coms);

    ~batlabTestPlan();

public slots:
    void onCreatePlan();
    void onStartTests();
    void onFinishedTests(int);
    batlabTestGroup* getNextTestGroup()
    {
        if (!testGroupList.isEmpty()) {
            return testGroupList.takeFirst();
        }
        return nullptr;
    }

signals:
    void emitAllTestsFinished();

private:
    int numberOfBatlabs = 0;
    QVector<batlabCell*> cellList;
    QVector<batlabTestGroup*> testGroupList;
    QVector<batlabCom*> comList;
    QVector<QThread*> threads;
};

#endif // BATLABTESTPLAN_H
