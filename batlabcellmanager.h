#ifndef BATLABCELLMANAGER_H
#define BATLABCELLMANAGER_H

#include <QObject>
#include <QMap>
#include "batlabcell.h"
#include "batlabcom.h"
#include "batlabtestplan.h"
#include <QDebug>



class batlabCellManager : public QObject
{
    Q_OBJECT

public:
    batlabCellManager();
    ~batlabCellManager();

public slots:
    void test();
//    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStream(int, int, int, float, float, float);

//    void onPrintCell(uchar key, properties val);
    void setProjectName(QString name);

    void onNewCell(QString id, testParms parms, double cap, int cycles);

    void onCreateTestPlan(int numBatlabs);
    void onCreateTestPlan(QVector<batlabCom*> coms);

    void onStartTests();
    void onAllTestsFinished();

    void onProcessCellData();

    void onSetNumberOfModules(int numModules) { numberOfModules = numModules; }
    void onSetNumberOfCellsPerModule(int numCells) { numberOfCellsPerModule = numCells; }
    void onSetVoltageCoefficient(float coef) { coefV = coef; }
    void onSetCurrentCoefficient(float coef) { coefI = coef; }
    void onSetChargeCoefficient(float coef) { coefSoC = coef; }

    void clean();

    batlabCell* onGetCell(int index) { return cellList.at(index); }

    batlabCell* onGetCell(QString designator)
    {
        for (int i = 0; i < cellList.size(); ++i) {
            if (cellList.at(i)->getDesignator() == designator) {
                return cellList.at(i);
            }
        }
        return nullptr;
    }

    QVector<batlabCell*> getCellList() { return cellList; }

    bool getTestsRunning() { return testsRunning; }

signals:
    void testFinished(uchar);
    void emitTests(QVector<int>*);
    void emitPack(QVector<QStringList>);

private:
    void saveLevelOneData(batlabCell* cellPointer);
    void interpolateData();
    QVector<batlabCell*> cellList;

    QVector<batlabCom*> comList;

    batlabTestPlan * testPlan = nullptr;

    QVector<QVector<float>> voltageTestData;
    QVector<QVector<float>> currentTestData;
    QVector<QVector<int>> chargeTestData;
    float **errorVoltage;
    float **errorCurrent;
    float **errorSoC;

    int numberOfBatlabs;

    int numberOfCellsPerModule = 0;
    int numberOfModules = 0;
    float coefV = 1.0;
    float coefI = 1.0;
    float coefSoC = 1.0;

    QString projectName;

    bool testsRunning = false;

};

#endif // BATLABCELLMANAGER_H
