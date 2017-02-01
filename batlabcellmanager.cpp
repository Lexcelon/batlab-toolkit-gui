#include "batlabcellmanager.h"

batlabCellManager::batlabCellManager()
{

}

batlabCellManager::~batlabCellManager()
{
    uchar temp;
    foreach(temp,cells.uniqueKeys()) {
        cells.remove(temp);
    }
}


void batlabCellManager::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {
        cells[key]->receiveStream( status, temp, current, voltage, charge);
    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onGetTests(uchar key) {
    if (cells.contains(key)) {
        emit emitTests(cells[key]->getTests());
    }
}

void batlabCellManager::onReceiveStreamExt(int unit,int cell,int currentAmp,int voltagePhase,int voltageAmp) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {
        cells[key]->receiveStreamExt( currentAmp,voltagePhase,voltageAmp);
    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onNewCell(uchar key) {
    cells.insert(key,new batlabCell(key));
    connect(cells[key],SIGNAL(testFinished(uchar)),this,SLOT(onTestFinished(uchar)));
}

void batlabCellManager::onNewCell(QString id , testParms parms, double ccr, double dcr, double cap, int cycles) {
    batlabCell * tempCell = new batlabCell(id,parms,cycles);
    cellList.push_back(tempCell);
}


void batlabCellManager::onTestFinished(uchar key) {
    emit testFinished(key);
}

void batlabCellManager::onNewTest(uchar key,uchar test) {
    cells[key]->newTest(test);
}

void batlabCellManager::onDeleteCell(uchar key) {
    cells.remove(key);
}

void batlabCellManager::onPrintCell(uchar key, properties val) {
    if (cells.contains(key))
    switch(val) {
    case properties::unit:
        qDebug() << QString("Unit: ") << cells[key]->getUnit();
        break;
    case properties::cell:
        qDebug() << QString("Cell: ") << cells[key]->getCell();
        break;
    case properties::status:
        qDebug() << QString("Status: ") << cells[key]->getStatus();
        break;
    case properties::statusString:
        qDebug() << QString("Status String: ") << cells[key]->getStatusString();
        break;
    case properties::temperature:
        qDebug() << QString("Temperature: ") << *cells[key]->getTemperature();
        break;
    case properties::voltage:
        qDebug() << QString("Voltage: ") << *cells[key]->getVoltage();
        break;
    case properties::current:
        qDebug() << QString("Current: ") << *cells[key]->getCurrent();
        break;
    case properties::charge:
        qDebug() << QString("Charge: ") << *cells[key]->getCharge();
        break;
    case properties::currentAmplitude:
        qDebug() << QString("Current Amplitude: ") << *cells[key]->getCurrentAmplitude();
        break;
    case properties::voltagePhase:
        qDebug() << QString("Voltage Phase: ") << *cells[key]->getVoltagePhase();
        break;
    case properties::voltageAmplitude:
        qDebug() << QString("Voltage Amplitude: ") << *cells[key]->getVoltageAmplitude();
        break;
    default:
        break;
    }
}

void batlabCellManager::onCreateTestPlan(int numBatlabs) {
    numberOfBatlabs = numBatlabs;
    testPlan = new batlabTestPlan(numBatlabs,cellList);
}


void batlabCellManager::onStartTests() {
    testPlan->onStartTests();
}
