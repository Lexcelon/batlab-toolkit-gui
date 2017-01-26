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

void batlabTestGroup::updateParms(int index) {

}
