#include "batlabcellmanager.h"
#include <QFile>

batlabCellManager::batlabCellManager()
{

}

batlabCellManager::~batlabCellManager()
{

}

void batlabCellManager::test()
{

    for (int cells = 0; cells < 10; ++cells) {
        cellList.push_back(new batlabCell);
        cellList[cells]->setDesignator(QString("PoopCell%1").arg(cells));
        QString name;
        name = QString("C:/Users/Seikowave/Desktop/TestCell/cell%1.btf").arg(cells+1);
        QFile inFile(name);
        inFile.open(QIODevice::ReadOnly);
        QTextStream stream(&inFile);
        voltageTestData.push_back(QVector<float>());
        currentTestData.push_back(QVector<float>());
        chargeTestData.push_back(QVector<int>());
        QString string;
        for (int i = 0; i < 20366; ++i) {
            stream >> string;
            voltageTestData[cells].push_back(string.toDouble());
//            (cellList[cells]->getVoltage())->push_back(string.toDouble());
        }
        for (int i = 0; i < 20366; ++i) {
            stream >> string;
            currentTestData[cells].push_back(string.toDouble());
//            (cellList[cells]->getCurrent())->push_back(string.toDouble());
        }
        for (int i = 0; i < 20366; ++i) {
            stream >> string;
            chargeTestData[cells].push_back(string.toInt());
//            (cellList[cells]->getSoC())->push_back(string.toDouble());
        }

    }
//    onProcessCellData();
qDebug() << Q_FUNC_INFO;
}

void batlabCellManager::onReceiveStream(int cell, int mode, int status, float temp, float current, float voltage)
{

}

void batlabCellManager::setProjectName(QString name)
{
    projectName = name;
}

void batlabCellManager::onNewCell(QString id , testParms parms, double cap, int cycles) {
    batlabCell * tempCell = new batlabCell(id,parms,cycles);
    cellList.push_back(tempCell);
}

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
    if (cellList.size() != 0) {
        testsRunning = true;
        testPlan->onStartTests();
    }
}

void batlabCellManager::onAllTestsFinished()
{
    for (int i = 0; i < cellList.size(); ++i) {
        saveLevelOneData(cellList[i]);
    }
    testsRunning = false;
}

void batlabCellManager::onProcessCellData()
{
    const int numCells = cellList.size();
//    interpolateData();
//    float coefV = 1.0;
//    float coefI = 1.0;
//    float coefSoC = 1.0;

//    int numberOfCellsInModule = 2;
//    int numberOfModules = 3;
    int numberOfCellsInPack = numberOfCellsPerModule * numberOfModules;

    errorVoltage = new float*[numCells];
    errorCurrent = new float*[numCells];
    errorSoC = new float*[numCells];
    QVector<QVector<float>> metric;
    metric.resize(numCells);

    for (int i = 0; i < numCells; ++i) {
        errorVoltage[i] = new float[numCells];
        errorCurrent[i] = new float[numCells];
        errorSoC[i] = new float[numCells];
        QVector<float> temp;
        temp.resize(numCells);
        metric[i] = temp;
    }

    QVector<float> means(numCells);
    QVector<int> ind;
    for (int i = 0; i < numCells; ++i) {
        ind.push_back(i);
    }

    QVector<QVector<int>> cellPreferenceIndices;

    for (int i = 0; i < numCells; ++i) {
        cellPreferenceIndices.push_back(QVector<int>(numCells));
        for (int j = 0; j < numCells; ++j) {
            cellPreferenceIndices[i][j] = j;
        }
    }

    for (int i = 0; i < numCells; i++) {
        for (int j = 0; j <= i; j++) {

            float sum = 0.0f;

            for (int k = 0; k < voltageTestData[i].size(); ++k) {
                float diff = (voltageTestData[i].at(k) - voltageTestData[j].at(k));
                diff *= diff;
                sum += diff;
            }

            errorVoltage[i][j] = sum;
            errorVoltage[j][i] = errorVoltage[i][j];
            errorVoltage[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < currentTestData[i].size(); ++k) {
                float diff = (currentTestData[i].at(k) - currentTestData[j].at(k));
                diff *= diff;
                sum += diff;
            }

            errorCurrent[i][j] = sum;
            errorCurrent[j][i] = errorCurrent[i][j];
            errorCurrent[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < chargeTestData[i].size(); ++k) {
                float diff = (chargeTestData[i].at(k) - chargeTestData[j].at(k));
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

    bubblesort(means, ind);

    for (int i = 0; i < numCells; ++i) {
        QVector<float> tempData;
        for (int j = 0; j < numCells; j++) {
            tempData.push_back(metric[j][i]);
        }
        bubblesort(tempData,(cellPreferenceIndices[i]));
    }

    int cellsLeftToPutInPack =  numberOfCellsInPack;
    int startingIndex = cellsLeftToPutInPack;

    QVector<QVector<float>> cellsInModule;
    for (int i = 0; i < numberOfModules; ++i) {
        QVector<float> temp;
        for (int j = 0; j < numberOfCellsPerModule; ++j) {
            temp.push_back(0);
        }
        cellsInModule.push_back(temp);
    }

    QVector<int> availableCells(ind);

    int cellToMatch = availableCells[startingIndex-1];
    int cellIndToMatch;
    int cellToRemove;

    QVector<QVector<int>> cCellPreferenceIndices(cellPreferenceIndices);


    for (int i = 0; i < numberOfModules; ++i) {

        for (int j = 0; j < numberOfCellsPerModule; ++j) {
            cellsInModule[i][j] = cCellPreferenceIndices[cellToMatch][j];
        }

        for (int k = 0; k < numCells; ++ k) {
            for (int j = 0; j < numberOfCellsPerModule; ++j) {
                cellToRemove = cellsInModule[i][j];
                cCellPreferenceIndices[k].removeAt(cCellPreferenceIndices[k].indexOf(cellToRemove));

                if (k == 0) {
                    availableCells.remove(availableCells.indexOf(cellToRemove));
                }
            }
        }


        cellsLeftToPutInPack = cellsLeftToPutInPack - numberOfCellsPerModule;
        if (cellsLeftToPutInPack > 0) {
            cellIndToMatch = cellsLeftToPutInPack;
            cellToMatch = availableCells[cellIndToMatch-1];
        }
    }

    qDebug() << cellsInModule;
    QVector<QStringList> module;
    for (int i = 0; i < cellsInModule.size(); ++i) {
        QStringList list;
        for (int j = 0; j < cellsInModule[i].size(); ++j) {
            list.push_back(cellList[cellsInModule[i][j]]->getDesignator());
        }
//        qDebug() << list;
        module.push_back(list);
    }
    emit emitPack(module);
//    qDebug() << cellsInModule;
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

    for (int i = 0; i < tempTests.size(); ++i) {
        QString filename = id;

        if (tempTests[i].REG_MODE.contains(MODE_CHARGE)) {
            filename += "Charge";
        } else if (tempTests[i].REG_MODE.contains(MODE_DISCHARGE)) {
            filename += "Discharge";
        }

        filename += tempTests[i].time.toString();
        QFile f( filename + QString(".bld") );

        if (f.open(QFile::Append | QIODevice::Text))
        {
            QTextStream data( &f );
            QStringList strList;

                strList.clear();
                strList << QString::number(tempTests[i].REG_MODE.size());
                for (int j = 0; j < tempTests[i].REG_MODE.size(); ++j) {
                    strList << QString::number(tempTests[i].REG_MODE[j]) ;
                }

                data << strList.join(",") << endl;
                strList.clear();

                strList << QString::number(tempTests[i].TIME.size());
                for (int j = 0; j < tempTests[i].TIME.size(); ++j) {
                    strList << QString::number(tempTests[i].TIME[j]) ;
                }

                data << strList.join(",") << endl;
                strList.clear();

                strList << QString::number(tempTests[i].REG_VOLTAGE.size());
                for (int j = 0; j < tempTests[i].REG_VOLTAGE.size(); ++j) {
                    strList <<  QString::number(tempTests[i].REG_VOLTAGE[j]);
                }

                data << strList.join(",") << endl;
                strList.clear();

                strList << QString::number(tempTests[i].REG_CURRENT.size());
                for (int j = 0; j < tempTests[i].REG_CURRENT.size(); ++j) {
                    strList << QString::number(tempTests[i].REG_CURRENT[j]);
                }

                data << strList.join(",") << endl;
                strList.clear();

                strList << QString::number(tempTests[i].REG_TEMPERATURE.size());
                for (int j = 0; j < tempTests[i].REG_TEMPERATURE.size(); ++j) {
                    strList << QString::number(tempTests[i].REG_TEMPERATURE[j]);
                }

                data << strList.join(",") << endl;
                strList.clear();

        }
        f.close();

        QString impedanceFilename;
        impedanceFilename = id + "Impedance" + tempTests[i].time.toString();
        QFile impedanceFile(impedanceFilename + ".bld");

        if (impedanceFile.open(QFile::Append | QIODevice::Text))
        {
            QTextStream data( &f );
            QStringList strList;

            for (int j = 0; j < tempTests[i].VOLTAGE_PP.size(); ++j) {
                strList << QString::number(tempTests[i].VOLTAGE_PP[j].first) << QString::number(tempTests[i].VOLTAGE_PP[j].second.first) << QString::number(tempTests[i].VOLTAGE_PP[j].second.second);
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].CURRENT_PP.size(); ++j) {
                strList << QString::number(tempTests[i].CURRENT_PP[j].first) <<  QString::number(tempTests[i].CURRENT_PP[j].second.first) << QString::number(tempTests[i].CURRENT_PP[j].second.second);
            }

            data << strList.join(",") << endl;
            strList.clear();
        }
        impedanceFile.close();

        QString chargeFilename;
        chargeFilename = id + "Coloumbs" + tempTests[i].time.toString();
        QFile chargeFile(chargeFilename + ".bld");

        if (chargeFile.open(QFile::Append | QIODevice::Text))
        {
            QTextStream data( &f );
            QStringList strList;

            for (int j = 0; j < tempTests[i].CHARGE.size(); ++j) {
                strList << QString::number(tempTests[i].CHARGE[j].first) << QString::number(tempTests[i].CHARGE[j].second) ;
            }

            data << strList.join(",") << endl;
            strList.clear();
        }
        chargeFile.close();

    }
}




void batlabCellManager::interpolateData()
{
    // create new time vector
    QVector<int> timeVector;

    //use max time
    int maxTime = 0;

    for (int i = 0; i < cellList.size(); ++i) {
        int time = cellList[i]->getTestData().last().TIME.last();
        if (time > maxTime) {
            maxTime = time;
        }
    }

    //fill time vector
    for (int i = 0; i < floor(maxTime/500); ++i) {
        timeVector.push_back(500 * i);
    }
    // interpolate data

    voltageTestData.resize(cellList.size());
    for (int i = 0; i < voltageTestData.size(); ++i) {
        voltageTestData[i].resize(timeVector.size());
    }

    for (int i = 0; i < voltageTestData.size(); ++i) {
        testPacket packet = cellList[i]->getTestData().last();
        voltageTestData[i].push_back(packet.REG_VOLTAGE.first());
        for (int j = 0; j < packet.REG_VOLTAGE.size() - 1; ++i) {
            int k = 0;

            if (packet.TIME[j] > (voltageTestData[i].size() * 500)) {
                float slope;
                slope = (packet.REG_VOLTAGE[j + 1] - packet.REG_VOLTAGE[j]) / static_cast<float>(1000);
                voltageTestData[i].push_back(slope * ((voltageTestData[i].size() * 500) - packet.TIME[j]));
                j--;
            }
        }

        currentTestData[i].push_back(packet.REG_CURRENT.first());
        for (int j = 0; j < packet.REG_CURRENT.size() - 1; ++i) {
            int k = 0;

            if (packet.TIME[j] > (currentTestData[i].size() * 500)) {
                float slope;
                slope = (packet.REG_CURRENT[j + 1] - packet.REG_CURRENT[j]) / static_cast<float>(1000);
                currentTestData[i].push_back(slope * ((currentTestData[i].size() * 500) - packet.TIME[j]));
                j--;
            }
        }

        int maxCharge = 0;
        for (int j = 0; j < packet.CHARGE.size(); ++j) {
            if (maxCharge < packet.CHARGE[j].second) {
                maxCharge = packet.CHARGE[j].second;
            }
        }

        chargeTestData[i].push_back(packet.CHARGE.first().second);
        for (int j = 0; j < packet.CHARGE.size() - 1; ++i) {
            int k = 0;

            if (packet.TIME[j] > (chargeTestData[i].size() * 500)) {
                float slope;
                slope = (packet.CHARGE[j + 1].second - packet.CHARGE[j].second) / static_cast<float>(1000);
                chargeTestData[i].push_back(slope * ((chargeTestData[i].size() * 500) - packet.TIME[j]) / maxCharge);
                j--;
            }
        }

    }


    // save/overwrite

}
