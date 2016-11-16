#include "batlabsystemtest.h"

batlabSystemTest::batlabSystemTest(QWidget *parent) :
    QObject(parent)
{

}

batlabSystemTest::~batlabSystemTest()
{

}


void batlabSystemTest::onRecieveTest(uchar number, uchar test)
{
    if (testSchedule.contains(number)) {
        testSchedule[number].push_back(test);
    } else {
        QVector<uchar> temp;
        temp.append(test);
        testSchedule[number] = temp;
    }
}

void batlabSystemTest::onGetTests(uchar number) {
    if(testSchedule.contains(number)) {
        emit emitTests(testSchedule[number]);
    }
}

void batlabSystemTest::onNextTest(uchar number) {
    if(testSchedule.contains(number)) {
        if (!testSchedule[number].isEmpty()) {
            emit emitNextTest(number,testSchedule[number].takeFirst());
        }
    }
}
