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

//    for (int cells = 0; cells < 10; ++cells) {
//        cellList.push_back(new batlabCell);
//        QString name;
//        name = QString("C:/Users/Seikowave/Desktop/TestCell/cell%1.btf").arg(cells+1);
//        QFile inFile(name);
//        inFile.open(QIODevice::ReadOnly);
//        QTextStream stream(&inFile);

//        QString string;
//        for (int i = 0; i < 20366; ++i) {
//            stream >> string;
//            (cellList[cells]->getVoltage())->push_back(string.toDouble());
//        }
//        for (int i = 0; i < 20366; ++i) {
//            stream >> string;
//            (cellList[cells]->getCurrent())->push_back(string.toDouble());
//        }
//        for (int i = 0; i < 20366; ++i) {
//            stream >> string;
//            (cellList[cells]->getSoC())->push_back(string.toDouble());
//        }

//    }
    onProcessCellData();

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

    int numberOfCellsInModule = 2;
    int numberOfModules = 3;
    int numberOfCellsInPack = numberOfCellsInModule * numberOfModules;

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

            for (int k = 0; k < cellList[i]->getTestData().last().REG_VOLTAGE.size(); ++k) {
                float diff = (cellList[i]->getTestData().last().REG_VOLTAGE.at(k) - cellList[j]->getTestData().last().REG_VOLTAGE.at(k));
                diff *= diff;
                sum += diff;
            }

            errorVoltage[i][j] = sum;
            errorVoltage[j][i] = errorVoltage[i][j];
            errorVoltage[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < cellList[i]->getTestData().last().REG_CURRENT.size(); ++k) {
                float diff = (cellList[i]->getTestData().last().REG_CURRENT.at(k) - cellList[j]->getTestData().last().REG_CURRENT.at(k));
                diff *= diff;
                sum += diff;
            }

            errorCurrent[i][j] = sum;
            errorCurrent[j][i] = errorCurrent[i][j];
            errorCurrent[i][i] = 0;

            sum = 0.0f;

            for (int k = 0; k < cellList[i]->getTestData().last().CHARGE.size(); ++k) {
                float diff = (cellList[i]->getTestData().last().CHARGE.at(k).second - cellList[j]->getTestData().last().CHARGE.at(k).second);
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
        for (int j = 0; j < numberOfCellsInModule; ++j) {
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

        for (int j = 0; j < numberOfCellsInModule; ++j) {
            cellsInModule[i][j] = cCellPreferenceIndices[cellToMatch][j];
        }

        for (int k = 0; k < numCells; ++ k) {
            for (int j = 0; j < numberOfCellsInModule; ++j) {
                cellToRemove = cellsInModule[i][j];
                cCellPreferenceIndices[k].removeAt(cCellPreferenceIndices[k].indexOf(cellToRemove));

                if (k == 0) {
                    availableCells.remove(availableCells.indexOf(cellToRemove));
                }
            }
        }


        cellsLeftToPutInPack = cellsLeftToPutInPack - numberOfCellsInModule;
        if (cellsLeftToPutInPack > 0) {
            cellIndToMatch = cellsLeftToPutInPack;
            cellToMatch = availableCells[cellIndToMatch-1];
        }
    }
    qDebug() << cellsInModule;
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


    QFile f( projectName + QString(".bld") );

    if (f.open(QFile::Append | QIODevice::Text))
    {
        QTextStream data( &f );
        QStringList strList;

        data << ".." + id << endl;

        for (int i = 0; i < tempTests.size(); ++i) {
            data << QString::number(tempTests[i].REG_MODE.first()) << endl;

            strList.clear();
            for (int j = 0; j < tempTests[i].TIME.size(); ++j) {
                strList << QString::number(tempTests[i].TIME[j]) ;
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_VOLTAGE.size(); ++j) {
                strList <<  QString::number(tempTests[i].REG_VOLTAGE[j]);
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_CURRENT.size(); ++j) {
                strList << QString::number(tempTests[i].REG_CURRENT[j]);
            }

            data << strList.join(",") << endl;
            strList.clear();

            for (int j = 0; j < tempTests[i].REG_TEMPERATURE.size(); ++j) {
                strList << QString::number(tempTests[i].REG_TEMPERATURE[j]);
            }

            data << strList.join(",") << endl;
            strList.clear();

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

            for (int j = 0; j < tempTests[i].CHARGE.size(); ++j) {
                strList << QString::number(tempTests[i].CHARGE[j].first) << QString::number(tempTests[i].CHARGE[j].second) ;
            }

            data << strList.join(",") << endl;
            strList.clear();
        }
    }
    f.close();
}
