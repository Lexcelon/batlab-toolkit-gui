#ifndef BATLABTESTGROUP_H
#define BATLABTESTGROUP_H

#include <QObject>
#include "globals.h"
#include "batlabcell.h"
#include "batlabcom.h"

class batlabTestGroup : public QObject
{
    Q_OBJECT

public:
    batlabTestGroup();
    batlabTestGroup(int batlabNumber);
    ~batlabTestGroup();

public slots:
    void onNewTest(batlabCell*newCell);
    int onGetSize();
    QString onGetCellID(int index);
    int onGetBatlabID();
    void onStartTests();
    void onSetBatlabID(int num) { batlabId = num; }
    bool onGetIsRunning() {return isRunning; }
    void updateParms(int index);

    void connectCom(batlabCom * com);
    void disconnectCom(batlabCom * com);

    void receiveStream(int cell, int mode, int stat, float temp, float curr, float volt);

//    void receiveStream(int,int,int,float,int,int,int);
//    void receiveStreamExt(int,int,int,int,int);

    void onTestFinished(int, QString id, int testNum);

    void startTests();

signals:
    void emitFinishedTests(int, QString, int testNum);
    void emitWriteReg(int,int,writeVals,int);

private:
    QVector<batlabCell*> testGroup;
    int batlabId = -1;
    bool isRunning = false;
    int count = 0x0000;
};

#endif // BATLABTESTGROUP_H
