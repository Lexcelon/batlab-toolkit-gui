#include "batlabtestgroup.h"
#include <QTimer>
#include <QMessageBox>

batlabTestGroup::batlabTestGroup(): QObject()
{
    count = 0;
    impedanceTimer = new QTimer(this);
    impedanceTimer->setSingleShot(true);
    connect(impedanceTimer, &QTimer::timeout,
            this, &batlabTestGroup::startImpedance);
}


batlabTestGroup::batlabTestGroup(int batlabNumber) : QObject(){
    batlabId = batlabNumber;
    count = 0;
    impedanceTimer = new QTimer(this);
    impedanceTimer->setSingleShot(true);
    connect(impedanceTimer, &QTimer::timeout,
            this, &batlabTestGroup::startImpedance);
}

batlabTestGroup::~batlabTestGroup()
{

}

void batlabTestGroup::onNewTest(batlabCell * newCell)
{
    testGroup.push_back(newCell);
    connect(newCell, &batlabCell::emitError,
            this, &batlabTestGroup::onError);
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
    int returnCode = QMessageBox::No;
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

void batlabTestGroup::connectCom(Batlab * com)
{
    comObject = com;
    serialNumber = com->getSerialNumber();
    connect(this,SIGNAL(emitWriteReg(int,int,int)),com,SLOT(onWriteReg(int,int,int)));
    connect(this,SIGNAL(emitReadReg(int,int)),com,SLOT(onReadReg(int,int)));
    connect(com,SIGNAL(emitReadResponse(int,int,int,int)), this, SLOT(receiveReadResponse(int,int,int,int)));
    connect(com,SIGNAL(emitWriteResponse(int,int,int,int)), this, SLOT(receiveWriteResponse(int,int,int)));
    connect(com,SIGNAL(newStreamReceived(int,int,int,float,float,float)), this, SLOT(receiveStream(int,int,int,float,float,float)));

    for (int i = 0; i < testGroup.size(); i++) {
        connect(testGroup[i],SIGNAL(updateParameter(int,int,int)),com,SLOT(onWriteReg(int,int,int)));
    }
}

void batlabTestGroup::disconnectCom()
{
    disconnect(this,SIGNAL(emitWriteReg(int,int,int)),comObject,SLOT(onWriteReg(int,int,int)));
    disconnect(this,SIGNAL(emitReadReg(int,int)),comObject,SLOT(onReadReg(int,int)));
    disconnect(comObject,SIGNAL(emitReadResponse(int,int,int,int)), this, SLOT(receiveReadResponse(int,int,int,int)));
    disconnect(comObject,SIGNAL(emitWriteResponse(int,int,int,int)), this, SLOT(receiveWriteResponse(int,int,int,int)));
    disconnect(comObject,SIGNAL(newStreamReceived(int,int,int,float,float,float)), this, SLOT(receiveStream(int,int,int,float,float,float)));

    for (int i = 0; i < testGroup.size(); i++) {
        disconnect(testGroup[i],SIGNAL(updateParameter(int,int,int,int)),comObject,SLOT(onWriteReg(int,int,int,int)));
    }
}

void batlabTestGroup::receiveStream(int cell, int mode, int stat, float temp, float curr, float volt)
{
    testGroup[cell]->receiveStream(mode, stat, temp, curr, volt);
}

void batlabTestGroup::receiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb)
{
    int value = static_cast<int>(lsb) | (static_cast<int>(msb) << 8);
    if (nameSpace == 4) {
        if (batlabRegister == unitNamespace::SINE_FREQ) {
            for (int i = 0; i < testGroup.size(); ++i) {
                testGroup[i]->setSineFreq(BatlabLib::getSineFrequency(value));
            }
        }
    } else if (nameSpace == 0 || nameSpace == 1 || nameSpace == 2 || nameSpace == 3 ) {
        testGroup[nameSpace]->receiveReadResponse(batlabRegister, value);
        if (batlabRegister == cellNamespace::CELL_ERROR) {
            switch (value) {
            case ERR_CURRENT_LIMIT_CHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            case ERR_CURRENT_LIMIT_DCHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            case ERR_VOLTAGE_LIMIT_CHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            case ERR_VOLTAGE_LIMIT_DCHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            case ERR_TEMP_LIMIT_CHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            case ERR_TEMP_LIMIT_DCHG:
                qDebug() << Q_FUNC_INFO << value;
                break;
            default:
                break;
            }
        }
    } else {

    }
}

//void batlabTestGroup::receiveWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb)
//{

//}

void batlabTestGroup::onTestFinished(int cell)
{
    qDebug() << Q_FUNC_INFO << cell;
//    emit emitFinishedTests(cell, id, testNum);
    count ^= (0x0001 << cell);
    emit emitWriteReg(cell, static_cast<int>(cellNamespace::REPORT_INTERVAL), BatlabLib::sendReportingFrequency(0.0f));

    emit emitWriteReg(cell, cellNamespace::LOCK, 1);
    emit emitReadReg(cell, cellNamespace::CHARGEL);
    emit emitReadReg(cell, cellNamespace::CHARGEH);
    emit emitWriteReg(cell, cellNamespace::LOCK, 0);

    emit emitWriteReg(cell, cellNamespace::MODE, MODE_STOPPED);
    if (count == 0x0000) {
        impedanceTimer->stop();
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
            testGroup[i]->onUpdateParameters(i);
            qDebug() << count << code;
            emit emitWriteReg(i,cellNamespace::CHARGEH,0);
            emit emitWriteReg(i,cellNamespace::CHARGEL,0);
            emit emitWriteReg(i,cellNamespace::MODE,code);
            emit emitWriteReg(i, static_cast<int>(cellNamespace::REPORT_INTERVAL), BatlabLib::sendReportingFrequency(testGroup[i]->onGetParameters().reportingFrequency));
            count = count ^ (0x0001 << i);
            qDebug() << count << code;
            testGroup[i]->onStartTimer();
        }
    }

    if (count == 0x0000) {
        isRunning = false;
        emit emitFinishedTests(batlabId);
    } else {
        impedanceTimer->start(30000);
    }
}

void batlabTestGroup::updateParms(int index)
{
//   testParms testParameters = testGroup[index]->onGetParameters();
   testGroup[index]->onUpdateParameters(index);
}

void batlabTestGroup::setChargeModes()
{
//    for (int i = 0; i < testGroup.size(); ++i) {
//        if (count & (0x0001 << i)) {
//            emit emitWriteReg(i,cellNamespace::MODE,MODE_CHARGE);
//        }
//    }

//    QTimer::singleShot(100,this,SLOT(onCheckImpedanceModes()));
}

void batlabTestGroup::onCheckChargeModes()
{
//    for (int i = 0; i < testGroup.size(); ++i) {
//        if (count & (0x0001 << i)) {
//            emit emitReadReg(i,cellNamespace::MODE);
//        }
//    }
//    QTimer::singleShot(100,this,SLOT(onVerifyImpedanceModes()));
}

void batlabTestGroup::onVerifyChargeModes()
{
    qDebug() << "This.";
    bool isOkay = true;
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            if (testGroup[i]->getCurrentMode() != MODE_CHARGE) {
                isOkay = false;
            }
        }
    }

    if (isOkay == false) {
        setChargeModes();
    } else {
        onReadCharge();
    }
}

void batlabTestGroup::onReadCharge()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            emit emitReadReg(i,cellNamespace::CHARGEH);
            emit emitReadReg(i,cellNamespace::CHARGEL);
        }
    }
}

void batlabTestGroup::startImpedance()
{
    onReadCharge();
    setImpedanceModes();
}

void batlabTestGroup::setImpedanceModes()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            emit emitWriteReg(i, static_cast<int>(cellNamespace::REPORT_INTERVAL), BatlabLib::sendReportingFrequency(0.0f));
            emit emitWriteReg(i,cellNamespace::MODE,MODE_IMPEDANCE);
        }
    }

    QTimer::singleShot(100,this,SLOT(onCheckImpedanceModes()));
}

void batlabTestGroup::onCheckImpedanceModes()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            emit emitReadReg(i,cellNamespace::MODE);
        }
    }
    QTimer::singleShot(100,this,SLOT(onVerifyImpedanceModes()));
}

void batlabTestGroup::onVerifyImpedanceModes()
{
    bool isOkay = true;
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            if (testGroup[i]->getCurrentMode() != MODE_IMPEDANCE) {
                isOkay = false;
            }
        }
    }

    if (isOkay == false) {
        setImpedanceModes();
    } else {
        freqCounter = 0;
        setFrequency();
    }
}

void batlabTestGroup::setFrequency()
{
    emit emitWriteReg(0x0004,unitNamespace::SINE_FREQ, 1 << freqCounter);

    QTimer::singleShot(100,this,SLOT(onCheckFrequency()));
}

void batlabTestGroup::onCheckFrequency()
{
    emit emitReadReg(0x0004,unitNamespace::SINE_FREQ);

    QTimer::singleShot(100,this,SLOT(onVerifyFrequency()));
}

void batlabTestGroup::onVerifyFrequency()
{
    bool isOkay = true;
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            if (BatlabLib::sendSineFrequency(testGroup[i]->getSineFreq()) != (1 << freqCounter)) {
                isOkay = false;
            }
        }
    }

    if (isOkay == false) {
        setFrequency();
    } else {
        QTimer::singleShot(1000,this,SLOT(onReadImpedance()));
    }
}

void batlabTestGroup::onReadImpedance()
{
    // risk here if the connections are not queued up right. We will want to change this so that the signals are the same.
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            emit emitWriteReg(i,cellNamespace::LOCK, 1);
            emit emitReadReg(i,cellNamespace::VOLTAGE_PP);
            emit emitReadReg(i,cellNamespace::CURRENT_PP);
            emit emitWriteReg(i,cellNamespace::LOCK, 0);
        }
    }

    freqCounter++;
    if (freqCounter > 3) {
        onRestartTests();
    } else {
        setFrequency();
    }
}

void batlabTestGroup::onRestartTests()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        int code = testGroup[i]->onGetNextTest();
        if (code != -1) {
            if (count & (0x0001 << i)) {
                emit emitWriteReg(i,cellNamespace::MODE,code);
                emit emitWriteReg(i, static_cast<int>(cellNamespace::REPORT_INTERVAL), BatlabLib::sendReportingFrequency(testGroup[i]->onGetParameters().reportingFrequency));
            }
        }
    }
    onCheckRestart();
}

void batlabTestGroup::onCheckRestart()
{
    for (int i = 0; i < testGroup.size(); ++i) {
        emit emitReadReg(i,cellNamespace::MODE);
    }

    QTimer::singleShot(100,this,SLOT(onVerifyRestart()));
}

void batlabTestGroup::onVerifyRestart()
{
    bool isOkay = true;
    for (int i = 0; i < testGroup.size(); ++i) {
        if (count & (0x0001 << i)) {
            if (testGroup[i]->getCurrentMode() != testGroup[i]->onGetNextTest()) {
                isOkay = false;
            }
        }
    }

    if (isOkay == false) {
        onRestartTests();
    } else {
        impedanceTimer->start(180000);
    }
}

void batlabTestGroup::onError(QString cellId)
{
    int index = 0;
    for (index = 0; index < testGroup.size(); ++index) {
        if (cellId == testGroup[index]->getDesignator()) {
            break;
        }
    }

    emit emitReadReg(index, cellNamespace::CELL_ERROR);
}
