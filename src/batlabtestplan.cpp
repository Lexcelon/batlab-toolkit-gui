#include "batlabtestplan.h"

batlabTestPlan::batlabTestPlan(): QObject()
{

}

batlabTestPlan::batlabTestPlan(int numBatlabs,QVector<batlabCell*> list): QObject() {
    numberOfBatlabs = numBatlabs;
    cellList = list;

    onCreatePlan();
}


batlabTestPlan::batlabTestPlan(int numBatlabs,QVector<batlabCell*> list, QVector<batlabCom*> coms): QObject()
{
    comList = coms;
    numberOfBatlabs = numBatlabs;
    cellList = list;

    onCreatePlan();
}

batlabTestPlan::~batlabTestPlan()
{

}


void batlabTestPlan::onCreatePlan()
{
    int numCells = cellList.size();
    int cellGroups = (int)ceil((double)numCells / 4.0f);

    for (int i = 0; i < comList.size(); ++i) {
        threads.push_back(new QThread());
    }

    for (int i = 0; i < cellGroups; i++) {
        testGroupList.push_back(new batlabTestGroup());

        int j = 0;
        for (j = 0; j < ((numCells>=4)?4:numCells); j++) {
            testGroupList[i]->onNewTest(cellList[i*4+j]);
        }
        numCells -= j;
    }
}

void batlabTestPlan::onStartTests()
{
//        int i = 0;

//        //old method used when we supported daisy chaining...could use in future
////        for (i = 0; i < ((testGroupList.size()>3)?3:testGroupList.size()); ++i) /{
//        for (i = 0; i < comList.size() && i < testGroupList.size(); i++)
//        {
//            connect(testGroupList[i],SIGNAL(emitFinishedTests(int)),this,SLOT(onFinishedTests(int)));
//            testGroupList[i]->onSetBatlabID(i);
//            testGroupList[i]->connectCom(comList[i]);
//            testGroupList[i]->onStartTests();

//        }
}

void batlabTestPlan::onFinishedTests(int val)
{
    testGroupList.removeOne(dynamic_cast<batlabTestGroup*>(sender()));
    int i = 0;
    while ((i < testGroupList.size()) && (i < numberOfBatlabs)) {
        if (testGroupList[i]->onGetIsRunning()) {
        } else {
            testGroupList[i]->disconnectCom();
            testGroupList[i]->onSetBatlabID(val);

            //dont think this is right...
            testGroupList[i]->connectCom(comList[val]);
            testGroupList[i]->onStartTests();

            connect(testGroupList[i],SIGNAL(emitFinishedTests()),this,SLOT(onFinishedTests()));
        }
        i++;
    }

    if (i == 0) {
        emit emitAllTestsFinished();
    }
}
