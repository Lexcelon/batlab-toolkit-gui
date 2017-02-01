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
    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStreamExt(int,int,int,int,int);
    void onNewCell(uchar);
    void onDeleteCell(uchar);
    void onPrintCell(uchar key, properties val);
    void onNewTest(uchar key,uchar test);
    void onTestFinished(uchar key);
    void onGetTests(uchar key);

    void onNewCell(QString id, testParms parms, double ccr, double dcr, double cap, int cycles);

    void onCreateTestPlan(int numBatlabs);
    void onStartTests();

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
    void emitTests(QVector<modeCodes>*);

private:
    QMap<uchar,batlabCell*> cells;
    QVector<batlabCell*> cellList;

    QVector<batlabCom*> comList;

    batlabTestPlan * testPlan;

    int numberOfBatlabs;

};

#endif // BATLABCELLMANAGER_H
