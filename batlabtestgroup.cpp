#include "batlabtestgroup.h"

batlabTestGroup::batlabTestGroup(): QObject()
{

}


batlabTestGroup::batlabTestGroup(int batlabNumber) : QObject(){
    batlabId = batlabNumber;
}

batlabTestGroup::~batlabTestGroup()
{

}

void batlabTestGroup::onNewTest(batlabCell * newTest) {
    testGroup.push_back(newTest);

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
    }
}


void batlabTestGroup::receiveStream(int unit, int cell, int stat,float temp,int curr, int volt,int cha) {
    testGroup[cell]->receiveStream(stat,temp,curr,volt,cha);
}


void batlabTestGroup::receiveStreamExt(int unit, int cell, int currAmp,int volPhase,int volAmp) {
    testGroup[cell]->receiveStreamExt(currAmp,volPhase,volAmp);
}


void batlabTestGroup::connectCom(batlabCom * com) {

    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(receiveStream(int,int,int,float,int,int,int)));
    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(receiveStreamExt(int,int,int,int,int)));

    for (int i = 0; i < testGroup.size(); i++) {
        connect(testGroup[i],SIGNAL(updateParameter(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
    }
}


void batlabTestGroup::updateParms(int index) {
   testGroup[index]->onUpdateParameters(0,index);
}
