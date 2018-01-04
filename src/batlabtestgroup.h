#ifndef BATLABTESTGROUP_H
#define BATLABTESTGROUP_H

#include <QObject>
#include "batlabcore/globals.h"
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
    void disconnectCom();

    void receiveStream(int cell, int mode, int stat, float temp, float curr, float volt);
    void receiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb);

//    void receiveStream(int,int,int,float,int,int,int);
//    void receiveStreamExt(int,int,int,int,int);

    void onTestFinished(int);

    void startTests();

    int onPromptStart();

    void startImpedance();
//    void startCharge();

    void setImpedanceModes();
    void onCheckImpedanceModes();
    void onVerifyImpedanceModes();

    void setChargeModes();
    void onCheckChargeModes();
    void onVerifyChargeModes();

    void setFrequency();
    void onCheckFrequency();
    void onVerifyFrequency();

    void onReadImpedance();
    void onReadCharge();

    void onRestartTests();
    void onCheckRestart();
    void onVerifyRestart();

    void onError(QString);

signals:
    void emitFinishedTests(int);
    void emitWriteReg(int,int,int);
    void emitReadReg(int,int);


private:
    QVector<batlabCell*> testGroup;
    batlabCom *comObject = nullptr;
    int serialNumber = -1;
    int batlabId = -1;
    bool isRunning = false;
    int count = 0x0000;
    QTimer *impedanceTimer;

    int freqCounter = 0;

};

#endif // BATLABTESTGROUP_H
