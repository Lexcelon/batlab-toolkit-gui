#include "batlabcellmanager.h"

batlabCellManager::batlabCellManager()
{

}

batlabCellManager::~batlabCellManager()
{

}


void batlabCellManager::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {

    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onReceiveStreamExt(int unit,int cell,int currentAmp,int voltagePhase,int voltageAmp) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {

    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onNewCell(uchar key) {
    cells.insert(key,new batlabCell(key));
}
