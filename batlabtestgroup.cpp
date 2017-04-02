#include "batlabtestgroup.h"
#include <QTimer>

batlabTestGroup::batlabTestGroup(): QObject()
{
    count = 0;
}


batlabTestGroup::batlabTestGroup(int batlabNumber) : QObject(){
    batlabId = batlabNumber;
    count = 0;
}

batlabTestGroup::~batlabTestGroup()
{

}

void batlabTestGroup::onNewTest(batlabCell * newCell) {
    testGroup.push_back(newCell);
}

int batlabTestGroup::onGetSize() {
    return testGroup.size();
}

int batlabTestGroup::onGetBatlabID() {
    return batlabId;
}

QString batlabTestGroup::onGetCellID(int index) {
    return testGroup[index]->getDesignator();
}

void batlabTestGroup::onStartTests() {
    isRunning = true;

    for (int i = 0; i < testGroup.size(); ++i) {
        updateParms(i);
        connect(testGroup[i],SIGNAL(testFinished(int)),this,SLOT(onTestFinished(int)));
    }

    startTests();
}

void batlabTestGroup::connectCom(batlabCom * com)
{
//    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(receiveStream(int,int,int,float,int,int,int)));
//    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(receiveStreamExt(int,int,int,int,int)));
    connect(this,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));

    connect(com,SIGNAL(emitStream(int,int,int,float,float,float)), this, SLOT(receiveStream(int,int,int,float,float,float)));

    for (int i = 0; i < testGroup.size(); i++) {
        connect(testGroup[i],SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    }
}

void batlabTestGroup::disconnectCom(batlabCom * com)
{
    disconnect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(receiveStream(int,int,int,float,int,int,int)));
    disconnect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(receiveStreamExt(int,int,int,int,int)));
    disconnect(this,SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    for (int i = 0; i < testGroup.size(); i++) {
        disconnect(testGroup[i],SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    }
}

void batlabTestGroup::receiveStream(int cell, int mode, int stat, float temp, float curr, float volt)
{
    testGroup[cell]->receiveStream(mode, stat, temp, curr, volt);
}

//void batlabTestGroup::receiveStream(int unit, int cell, int stat,float temp,int curr, int volt,int cha)
//{
//    testGroup[cell]->receiveStream(stat,temp,curr,volt,cha);
//}

//void batlabTestGroup::receiveStreamExt(int unit, int cell, int currAmp,int volPhase,int volAmp)
//{
//    testGroup[cell]->receiveStreamExt(currAmp,volPhase,volAmp);
//}

void batlabTestGroup::onTestFinished(int cell, QString id, int testNum)
{
    emit emitFinishedTests(cell, id, testNum);
    count ^= (0x0001 << cell);

    if (count == 0x0000) {
        int mSecRest = 0;
        for (int i = 0; i < testGroup.size(); ++i) {
            if (mSecRest < testGroup[i]->onGetParameters().restTime * 1000)
                mSecRest = testGroup[i]->onGetParameters().restTime * 1000;
        }
       QTimer::singleShot(mSecRest, this, SLOT(startTests()));
//        startTests();
    }
}

void batlabTestGroup::startTests()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        int code = testGroup[i]->onGetNextTest();
        if (code != -1) {
            emit emitWriteReg(0,i,writeVals::mode,code);
            emit emitWriteReg(0,i,writeVals::command,commandCodes::start);
            count = count ^ (0x0001 << i);
        }
    }
}

void batlabTestGroup::updateParms(int index)
{
   testParms testParameters = testGroup[index]->onGetParameters();

   emit emitWriteReg(0, index, writeVals::highTempChargeSafetyCutoff, testParameters.temperatureCutoffCharge);
   emit emitWriteReg(0, index, writeVals::streamReportingPeriod, testParameters.reportingFrequency);
}
