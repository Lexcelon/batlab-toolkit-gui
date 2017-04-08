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
//    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStream(int, int, int, float, float, float);

//    void onPrintCell(uchar key, properties val);

    void onNewCell(QString id, testParms parms, double ccr, double dcr, double cap, int cycles);

    void onCreateTestPlan(int numBatlabs);
    void onCreateTestPlan(QVector<batlabCom*> coms);

    void onStartTests();
    void onAllTestsFinished();

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


signals:
    void testFinished(uchar);
    void emitTests(QVector<int>*);

private:
    void saveLevelOneData(batlabCell* cellPointer);
    QVector<batlabCell*> cellList;

    QVector<batlabCom*> comList;

    batlabTestPlan * testPlan = nullptr;

    int numberOfBatlabs;

};

#endif // BATLABCELLMANAGER_H
