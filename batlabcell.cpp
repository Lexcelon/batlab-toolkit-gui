#include "batlabcell.h"

batlabCell::batlabCell()
{
    for (int i = 0; i < 100; ++i) {
        temperature.append(i);
    }
}

batlabCell::batlabCell(uchar key)
{
    unit = key >> 2;
    cell = key & 0x03;
}

batlabCell::~batlabCell()
{

}

void batlabCell::receiveStream(int stat,float temp,int curr, int volt,int cha) {
    status = stat;
    statusString = parseStatus(stat);
    temperature.append(temp);
    current.append(curr);
    voltage.append(volt);
    charge.append(cha);
}


void batlabCell::receiveStreamExt(int currAmp,int volPhase,int volAmp) {
    currentAmplitude.append(currAmp);
    voltagePhase.append(volPhase);
    voltageAmplitude.append(volAmp);
}

