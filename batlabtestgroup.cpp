#include "batlabtestgroup.h"

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

    for (int i = 0; i < testGroup.size(); ++i) {
        int code = testGroup[i]->onGetNextTest();
        emit emitWriteReg(0,i,writeVals::mode,code);
        emit emitWriteReg(0,i,writeVals::command,commandCodes::start);
        count = count ^ (0x0001 << i);
    }
}

void batlabTestGroup::connectCom(batlabCom * com)
{
    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(receiveStream(int,int,int,float,int,int,int)));
    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(receiveStreamExt(int,int,int,int,int)));
    connect(this,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
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

void batlabTestGroup::receiveStream(int unit, int cell, int stat,float temp,int curr, int volt,int cha)
{
    testGroup[cell]->receiveStream(stat,temp,curr,volt,cha);
}

void batlabTestGroup::receiveStreamExt(int unit, int cell, int currAmp,int volPhase,int volAmp)
{
    testGroup[cell]->receiveStreamExt(currAmp,volPhase,volAmp);
}

void batlabTestGroup::onTestFinished(int cell, QString id, int testNum)
{
    emit emitFinishedTests(cell, id, testNum);
    count ^= (0x0001 << cell);

    if (count == 0x0000) {
        for (int i = 0; i < testGroup.size(); ++i) {
            int code = testGroup[i]->onGetNextTest();
            if (code != -1) {
                emit emitWriteReg(0,i,writeVals::mode,code);
                emit emitWriteReg(0,i,writeVals::command,commandCodes::start);
                count = count ^ (0x0001 << i);
            }
        }
    }
}

void batlabTestGroup::updateParms(int index)
{
   testParms testParameters = testGroup[index]->onGetParameters();

   emit emitWriteReg(0, index, writeVals::dischargeCurrentSafetyCutoff,testParameters.dcsc);
   emit emitWriteReg(0, index, writeVals::highTempChargeSafetyCutoff, testParameters.highTemperatureCutoff);
   emit emitWriteReg(0, index, writeVals::pulseOffTime,testParameters.poft);
   emit emitWriteReg(0, index, writeVals::pulseOnTime, testParameters.pont);
   emit emitWriteReg(0, index, writeVals::sinewaveFrequency,testParameters.sinewaveFrequency);
   emit emitWriteReg(0, index, writeVals::streamReportingPeriod, testParameters.reportingFrequency);
}