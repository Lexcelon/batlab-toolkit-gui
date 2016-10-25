#include "batlabcellmanager.h"

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


void batlabCellManager::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {
        cells[key]->receiveStream( status, temp, current, voltage, charge);
    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onReceiveStreamExt(int unit,int cell,int currentAmp,int voltagePhase,int voltageAmp) {
    uchar key = uchar(((unit<<2) + cell));
    if (cells.contains(key)) {
        cells[key]->receiveStreamExt( currentAmp,voltagePhase,voltageAmp);
    } else {
        onNewCell(key);
    }
}

void batlabCellManager::onNewCell(uchar key) {
    cells.insert(key,new batlabCell(key));
}

void batlabCellManager::onDeleteCell(uchar key) {
    cells.remove(key);
}


