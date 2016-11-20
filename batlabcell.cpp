#include "batlabcell.h"

batlabCell::batlabCell()
{
}

batlabCell::batlabCell(uchar key)
{
    unit = key >> 2;
    cell = key & 0x03;
}

batlabCell::batlabCell(QString designator, QString number)
{
    name = designator;
    cellNumber = number;
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

    if (!tests.isEmpty()) {
        tests.last().temperature.append(temp);
        tests.last().current.append(curr);
        tests.last().voltage.append(volt);
        tests.last().charge.append(cha);
    }

    if (status & 0x01) {
        emit testFinished((unit << 2)|cell);
    }
}


void batlabCell::receiveStreamExt(int currAmp,int volPhase,int volAmp) {
    currentAmplitude.append(currAmp);
    voltagePhase.append(volPhase);
    voltageAmplitude.append(volAmp);

    if (!tests.isEmpty()) {
        tests.last().currentAmplitude.append(currAmp);
        tests.last().voltagePhase.append(volPhase);
        tests.last().voltageAmplitude.append(volAmp);
    }
}

void batlabCell::newTest(uchar testnum) {
    test newTest;
    newTest.mode = testnum;
    tests.push_back(newTest);
}
