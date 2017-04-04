#include "batlabcellmanager.h"
#include <QFile>

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


//void batlabCellManager::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
//    uchar key = uchar(((unit<<2) + cell));
//    if (cells.contains(key)) {
//        cells[key]->receiveStream( status, temp, current, voltage, charge);
//    } else {
//        onNewCell(key);
//    }
//}

void batlabCellManager::onReceiveStream(int cell, int mode, int status, float temp, float current, float voltage) {
    uchar key = uchar(cell);
    if (cells.contains(key)) {
        cells[key]->receiveStream( mode, status, temp, current, voltage);
    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onGetTests(uchar key) {
    if (cells.contains(key)) {
        emit emitTests(cells[key]->getTests());
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
//    cells[key]->newTest(test);
}

void batlabCellManager::onDeleteCell(uchar key) {
    cells.remove(key);
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

void batlabCellManager::saveLevelOneData(batlabCell* cellPointer)
{
    QString id = cellPointer->getDesignator();
    testParms tempParms = cellPointer->onGetParameters();
    QVector<testPacket> tempTests = cellPointer->getTestData();


    QFile f( "projectName.blp" );

    if (f.open(QFile::Append))
    {
        QTextStream data( &f );
        QStringList strList;

        data << ".." + id + "\n";

        for (int i = 0; i < tempTests.size(); ++i) {
            data << QString::number(tempTests[i].REG_MODE.first()) << "\n";

            strList.clear();
            for (int j = 0; j < tempTests[i].TIME.size(); ++j) {
                strList << " " << QString::number(tempTests[i].TIME[j]) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_VOLTAGE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_VOLTAGE[j]) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_CURRENT.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_CURRENT[j]) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_TEMPERATURE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].REG_TEMPERATURE[j]) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].VOLTAGE_PP.size(); ++j) {
                strList << " " << QString::number(tempTests[i].VOLTAGE_PP[j].first) << " " << QString::number(tempTests[i].VOLTAGE_PP[j].second.first) << " " << QString::number(tempTests[i].VOLTAGE_PP[j].second.second) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].VOLTAGE_PHASE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].VOLTAGE_PHASE[j].first) << " " << QString::number(tempTests[i].VOLTAGE_PHASE[j].second.first) << " " << QString::number(tempTests[i].VOLTAGE_PHASE[j].second.second) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].CURRENT_PP.size(); ++j) {
                strList << " " << QString::number(tempTests[i].CURRENT_PP[j].first) << " " << QString::number(tempTests[i].CURRENT_PP[j].second.first) << " " << QString::number(tempTests[i].CURRENT_PP[j].second.second) << " ";
            }

            data << strList.join(",") + "\n";
            strList.clear();

            for (int j = 0; j < tempTests[i].CURRENT_PHASE.size(); ++j) {
                strList << " " << QString::number(tempTests[i].CURRENT_PHASE[j].first) << " " << QString::number(tempTests[i].CURRENT_PHASE[j].second.first) << " " << QString::number(tempTests[i].CURRENT_PHASE[j].second.second) << " ";
            }
            data << strList.join(",") + "\n";
            strList.clear();
        }
    }
    f.close();
}
