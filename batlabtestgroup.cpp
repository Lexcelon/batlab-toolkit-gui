#include "batlabtestgroup.h"
#include <QTimer>
#include <QMessageBox>

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

void batlabTestGroup::onNewTest(batlabCell * newCell)
{
    testGroup.push_back(newCell);
}

int batlabTestGroup::onGetSize()
{
    return testGroup.size();
}

int batlabTestGroup::onGetBatlabID()
{
    return batlabId;
}

QString batlabTestGroup::onGetCellID(int index)
{
    return testGroup[index]->getDesignator();
}

int batlabTestGroup::onPromptStart()
{
    int returnCode;
    switch (testGroup.size()) {
    case 1:
        returnCode = QMessageBox::warning(0, tr("Starting Test"), tr("Please insert cell %1 into slots 0 for Batlab %2.").arg(testGroup[0]->getDesignator()).arg(serialNumber), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        break;
    case 2:
        returnCode = QMessageBox::warning(0, tr("Starting Test"), tr("Please insert cells %1 and %2 into slots 0 and 1, respectively,  for Batlab %3.").arg(testGroup[0]->getDesignator()).arg(testGroup[1]->getDesignator()).arg(serialNumber), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        break;
    case 3:
        returnCode = QMessageBox::warning(0, tr("Starting Test"), tr("Please insert cells %1, %2, and %3 into slots 0, 1, and 2, respectively,  for Batlab %4.").arg(testGroup[0]->getDesignator()).arg(testGroup[1]->getDesignator()).arg(testGroup[2]->getDesignator()).arg(serialNumber), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        break;
    case 4:
        returnCode = QMessageBox::warning(0, tr("Starting Test"), tr("Please insert cells %1, %2, %3, and %4 into slots 0, 1, 2, and 3, respectively,  for Batlab %5.").arg(testGroup[0]->getDesignator()).arg(testGroup[1]->getDesignator()).arg(testGroup[2]->getDesignator()).arg(testGroup[3]->getDesignator()).arg(serialNumber), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
        break;
    default:
        break;
    }
    return returnCode;
}


void batlabTestGroup::onStartTests()
{
    int ret = onPromptStart();
    if (ret == QMessageBox::Yes) {
        isRunning = true;
        for (int i = 0; i < testGroup.size(); ++i) {
            updateParms(i);
            connect(testGroup[i],SIGNAL(testFinished(int)),this,SLOT(onTestFinished(int)));
        }

        startTests();
    }
}

void batlabTestGroup::connectCom(batlabCom * com)
{
    comObject = com;
    connect(this,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    connect(com,SIGNAL(emitResponse(int,int,int,int)), this, SLOT(receiveResponse(int,int,int,int)));
    connect(com,SIGNAL(emitStream(int,int,int,float,float,float)), this, SLOT(receiveStream(int,int,int,float,float,float)));

    for (int i = 0; i < testGroup.size(); i++) {
        connect(testGroup[i],SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    }
}

void batlabTestGroup::disconnectCom(batlabCom * com)
{
    disconnect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(receiveStream(int,int,int,float,int,int,int)));
    disconnect(this,SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    for (int i = 0; i < testGroup.size(); i++) {
        disconnect(testGroup[i],SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    }
}

void batlabTestGroup::receiveStream(int cell, int mode, int stat, float temp, float curr, float volt)
{
    testGroup[cell]->receiveStream(mode, stat, temp, curr, volt);
}

void batlabTestGroup::receiveResponse(int nameSpace, int batlabRegister, int lsb, int msb)
{
    int value = static_cast<int>(lsb) | (static_cast<int>(msb) << 8);
    if (nameSpace == 4) {
        if (batlabRegister == unitNamespace::SINE_FREQ) {
            for (int i = 0; i < testGroup.size(); ++i) {
                testGroup[i]->setSineFreq(static_cast<float>(value) * static_cast<float>(10000)/static_cast<float>(256));
            }
        }
    } else if (nameSpace == 0 || nameSpace == 1 || nameSpace == 2 || nameSpace == 3 ) {
//        if (batlabRegister == cellNamespace::)
    } else {

    }
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
            if (mSecRest < testGroup[i]->onGetParameters().restTime * 1000) {
                mSecRest = testGroup[i]->onGetParameters().restTime * 1000;
            }
        }
       QTimer::singleShot(mSecRest, this, SLOT(startTests()));
    }
}

void batlabTestGroup::startTests()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        int code = testGroup[i]->onGetNextTest();
        if (code != -1) {
//            emit emitWriteReg(0,i,writeVals::mode,code);
//            emit emitWriteReg(0,i,writeVals::command,commandCodes::start);
            count = count ^ (0x0001 << i);
        }
    }
}

void batlabTestGroup::updateParms(int index)
{
   testParms testParameters = testGroup[index]->onGetParameters();

//   emit emitWriteReg(0, index, writeVals::highTempChargeSafetyCutoff, testParameters.temperatureCutoffCharge);
//   emit emitWriteReg(0, index, writeVals::streamReportingPeriod, testParameters.reportingFrequency);
}
