#include "batlabtestplan.h"

batlabTestPlan::batlabTestPlan(): QObject()
{

}

batlabTestPlan::batlabTestPlan(int numBatlabs,QVector<batlabCell*> list): QObject() {
    numberOfBatlabs = numBatlabs;
    cellList = list;

    onCreatePlan();
}


batlabTestPlan::~batlabTestPlan()
{

}


void batlabTestPlan::onCreatePlan() {
    int numCells = cellList.size();
    int cellGroups = (int)ceil((double)numCells / 4.0f);

    for (int i = 0; i < cellGroups; i++) {
        testGroupList.push_back(new batlabTestGroup());

        int j = 0;
        for (j = 0; j < ((numCells>=4)?4:numCells); j++) {
            testGroupList[i]->onNewTest(cellList[i*4+j]);
        }
        numCells -= j;
    }


//    for (int i = 0; i < testGroupList.size(); ++i) {
//        qDebug() << testGroupList[i]->onGetBatlabID();
//        for (int j = 0; j < testGroupList[i]->onGetSize(); j++) {
//            qDebug() << testGroupList[i]->onGetCellID(j);
//        }
//    }
}

void batlabTestPlan::onStartTests() {
        int i = 0;
        for (i = 0; i < ((testGroupList.size()>3)?3:testGroupList.size()); ++i) {
            testGroupList[i]->onSetBatlabID(i);
            testGroupList[i]->onStartTests();

            connect(testGroupList[i],SIGNAL(emitFinishedTests(int)),this,SLOT(onFinishedTests(int)));
        }
}

void batlabTestPlan::onFinishedTests(int val) {
    testGroupList.removeAt(val);
    int i = 0;
    while (i < testGroupList.size()) {
        if (testGroupList[i]->onGetIsRunning()) {
            i++;
        } else {
            testGroupList[i]->onSetBatlabID(val);
            testGroupList[i]->onStartTests();
            connect(testGroupList[i],SIGNAL(emitFinishedTests(int)),this,SLOT(onFinishedTests(int)));
        }
    }
}
