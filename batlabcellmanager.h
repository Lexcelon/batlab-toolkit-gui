#ifndef BATLABCELLMANAGER_H
#define BATLABCELLMANAGER_H

#include <QObject>
#include <QMap>
#include "batlabcell.h"
#include <QDebug>

enum class properties {
    unit,
    cell,
    status,
    statusString,
    temperature,
    current,
    voltage,
    charge,
    currentAmplitude,
    voltagePhase,
    voltageAmplitude
};

class batlabCellManager : public QObject
{
    Q_OBJECT

public:
    batlabCellManager();
    ~batlabCellManager();
    void print(uchar key, properties val);

public slots:
    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStreamExt(int,int,int,int,int);
    void onNewCell(uchar);
    void onDeleteCell(uchar);

private:
    QMap<uchar,batlabCell*> cells;
};

#endif // BATLABCELLMANAGER_H
