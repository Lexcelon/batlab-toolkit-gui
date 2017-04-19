#include "batlabcellmanager.h"
#include <QFile>

batlabCellManager::batlabCellManager()
{

}

batlabCellManager::~batlabCellManager()
{

}

void batlabCellManager::onReceiveStream(int cell, int mode, int status, float temp, float current, float voltage)
{

}

void batlabCellManager::onNewCell(QString id , testParms parms, double cap, int cycles) {
    batlabCell * tempCell = new batlabCell(id,parms,cycles);
    cellList.push_back(tempCell);
}

//void batlabCellManager::onPrintCell(uchar key, properties val) {
//    if (cells.contains(key))
//    switch(val) {
//    case properties::unit:
//        qDebug() << QString("Unit: ") << cells[key]->getUnit();
//        break;
//    case properties::cell:
//        qDebug() << QString("Cell: ") << cells[key]->getCell();
//        break;
//    case properties::status:
//        qDebug() << QString("Status: ") << cells[key]->getStatus();
//        break;
//    case properties::statusString:
//        qDebug() << QString("Status String: ") << cells[key]->getStatusString();
//        break;
//    case properties::temperature:
//        qDebug() << QString("Temperature: ") << *cells[key]->getTemperature();
//        break;
//    case properties::voltage:
//        qDebug() << QString("Voltage: ") << *cells[key]->getVoltage();
//        break;
//    case properties::current:
//        qDebug() << QString("Current: ") << *cells[key]->getCurrent();
//        break;
//    case properties::charge:
//        qDebug() << QString("Charge: ") << *cells[key]->getCharge();
//        break;
//    case properties::currentAmplitude:
//        qDebug() << QString("Current Amplitude: ") << *cells[key]->getCurrentAmplitude();
//        break;
//    case properties::voltagePhase:
//        qDebug() << QString("Voltage Phase: ") << *cells[key]->getVoltagePhase();
//        break;
//    case properties::voltageAmplitude:
//        qDebug() << QString("Voltage Amplitude: ") << *cells[key]->getVoltageAmplitude();
//        break;
//    default:
//        break;
//    }
//}

void batlabCellManager::onCreateTestPlan(int numBatlabs) {
    numberOfBatlabs = numBatlabs;
    testPlan = new batlabTestPlan(numBatlabs,cellList);
    connect(testPlan, SIGNAL(emitAllTestsFinished()), this, SLOT(onAllTestsFinished()));
}

void batlabCellManager::onCreateTestPlan(QVector<batlabCom*> coms)
{
    numberOfBatlabs = coms.size();
    testPlan = new batlabTestPlan(numberOfBatlabs, cellList, coms);
    connect(testPlan, SIGNAL(emitAllTestsFinished()), this, SLOT(onAllTestsFinished()));
}


void batlabCellManager::onStartTests() {
    testPlan->onStartTests();
}

void batlabCellManager::onAllTestsFinished()
{
    for (int i = 0; i < cellList.size(); ++i) {
        saveLevelOneData(cellList[i]);
    }
}

void batlabCellManager::onProcessCellData()
{
    const int numCells = cellList.size();

    float coefV = 1.0;
    float coefI = 1.0;
    float coefSoC = 1.0;

    errorVoltage = new float*[numCells];
    errorCurrent = new float*[numCells];
    errorSoC = new float*[numCells];
    float **metric = new float*[numCells];

    for (int i = 0; i < numCells; ++i) {
        errorVoltage[i] = new float[numCells];
        errorCurrent[i] = new float[numCells];
        errorSoC[i] = new float[numCells];
        metric[i] = new float[numCells];
    }

    float *means = new float[numCells];
    float *indices = new float[numCells];

    for (int i = 0; i < numCells; i++) {
        for (int j = 0; j <= i; j++) {

            float sum = 0.0f;

            for (int k = 0; k < cellList[i]->getVoltage()->size(); ++k) {
                float diff = (cellList[i]->getVoltage())->at(k) - (cellList[j]->getVoltage())->at(k);
                diff *= diff;
                sum += diff;
            }

            errorVoltage[i][j] = sum;
            errorVoltage[j][i] = errorVoltage[i][j];
            errorVoltage[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < cellList[i]->getCurrent()->size(); ++k) {
                float diff = (cellList[i]->getCurrent())->at(k) - (cellList[j]->getCurrent())->at(k);
                diff *= diff;
                sum += diff;
            }

            errorCurrent[i][j] = sum;
            errorCurrent[j][i] = errorCurrent[i][j];
            errorCurrent[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < cellList[i]->getSoC()->size(); ++k) {
                float diff = (cellList[i]->getSoC())->at(k) - (cellList[j]->getSoC())->at(k);
                diff *= diff;
                sum += diff;
            }
            errorSoC[i][j] = sum;
            errorSoC[j][i] = errorSoC[i][j];
            errorSoC[i][i] = 0;
        }
    }



    for (int i = 0; i < numCells; i++) {
        for (int j = 0; j <= i ; j++) {
            metric[i][j] = coefV * errorVoltage[i][j] + coefI * errorCurrent[i][j] + coefSoC * errorSoC[i][j];
            metric[j][i] = coefV * errorVoltage[j][i] + coefI * errorCurrent[j][i] + coefSoC * errorSoC[j][i];
        }
        means[i] = 0.0f;
    }



    for (int i = 0; i < numCells; ++i) {
        for (int j = 0; j < numCells; ++j) {
            means[i] += metric[j][i];
        }
        means[i] /= numCells;
    }


    std::sort(means, means + numCells - 1, std::less<float>());
}

void batlabCellManager::clean()
{
    while (!cellList.isEmpty()) {
        if (cellList.first() != nullptr) {
            delete cellList.takeFirst();
        }
    }

    if (testPlan) {
        delete testPlan;
        testPlan = nullptr;
    }
}

void batlabCellManager::saveLevelOneData(batlabCell* cellPointer)
{
    QString id = cellPointer->getDesignator();
    testParms tempParms = cellPointer->onGetParameters();
    QVector<testPacket> tempTests = cellPointer->getTestData();


    QFile f( "projectName.bld" );

    if (f.open(QFile::Append | QIODevice::Text))
    {
        QTextStream data( &f );
        QStringList strList;

        data << ".." + id << endl;

        for (int i = 0; i < tempTests.size(); ++i) {
            data << QString::number(tempTests[i].REG_MODE.first()) << endl;

            strList.clear();
            for (int j = 0; j < tempTests[i].TIME.size(); ++j) {
                strList << " " << QString::number(tempTests[i].TIME[j]) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_VOLTAGE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_VOLTAGE[j]) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_CURRENT.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_CURRENT[j]) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_TEMPERATURE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_TEMPERATURE[j]) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].VOLTAGE_PP.size(); ++j) {
                strList << " " << QString::number(tempTests[i].VOLTAGE_PP[j].first) << " " << QString::number(tempTests[i].VOLTAGE_PP[j].second.first) << " " << QString::number(tempTests[i].VOLTAGE_PP[j].second.second) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].CURRENT_PP.size(); ++j) {
                strList << " " << QString::number(tempTests[i].CURRENT_PP[j].first) << " " << QString::number(tempTests[i].CURRENT_PP[j].second.first) << " " << QString::number(tempTests[i].CURRENT_PP[j].second.second) << " ";
            }

            data << strList.join(",") << endl;
            strList.clear();
        }
    }
    f.close();
}
