#ifndef BATLABCELLMANAGER_H
#define BATLABCELLMANAGER_H

#include <QObject>
#include <QMap>
#include "batlabcell.h"

class batlabCellManager
{
    Q_OBJECT

public:
    batlabCellManager();
    ~batlabCellManager();

public slots:
    void onReceiveStream(int,int,int,float,int,int,int);
    void onReceiveStreamExt(int,int,int,int,int);
    void onNewCell(uchar);

private:
    QMap<uchar,batlabCell*> cells;
};

#endif // BATLABCELLMANAGER_H
