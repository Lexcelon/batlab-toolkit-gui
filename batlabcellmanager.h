#ifndef BATLABCELLMANAGER_H
#define BATLABCELLMANAGER_H

#include <QObject>
#include <QMap>
#include "batlabcell.h"
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

    void onNewCell(QString name , QString number);

signals:
    void testFinished(uchar);
    void emitTests(QVector<test>*);

private:
    QMap<uchar,batlabCell*> cells;
};

#endif // BATLABCELLMANAGER_H