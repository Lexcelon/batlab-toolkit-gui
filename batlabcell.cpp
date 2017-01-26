#include "batlabcell.h"

batlabCell::batlabCell()
{
}

batlabCell::batlabCell(uchar key)
{
    unit = key >> 2;
    cell = key & 0x03;
}

batlabCell::batlabCell(QString designator, testParms parms, int cycles)
{
    id = designator;
    testParameters = parms;

    if (cycles > 0) {
        testsToRun.push_back(ccCharge);
    }

    for (int i = 0; i < cycles; i++) {
        testsToRun.push_back(ccDischarge);
        testsToRun.push_back(ccCharge);
    }
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

void batlabCell::onUpdateParameters(int unit, int cell)
{

//    emit updateParameter(unit, cell, writeVals::chargeCurrentCutoff,testParameters.cc);
//    emit updateParameter(unit, cell, writeVals::dischargeCurrentCutoff);
//    emit updateParameter(unit, cell, writeVals::chargeCurrentSafetyCutoff,testParameters.chargeCurrentSafetyCutoff);
    emit updateParameter(unit, cell, writeVals::dischargeCurrentSafetyCutoff,testParameters.dcsc);
//    emit updateParameter(unit, cell, writeVals::chargeCVCurrentCutoff,testParameters.);
//    emit updateParameter(unit, cell, writeVals::dischargeCVCurrentCutoff);
//    emit updateParameter(unit, cell, writeVals::chargeVoltageCutoff, testParameters.);
//    emit updateParameter(unit, cell, writeVals::dischargeVoltageCutoff, testParameters.c);
    emit updateParameter(unit, cell, writeVals::highTempChargeSafetyCutoff, testParameters.highTemperatureCutoff);
//    emit updateParameter(unit, cell, writeVals::highTempDischargeSafetyCutoff);
    emit updateParameter(unit, cell, writeVals::pulseOffTime,testParameters.poft);
    emit updateParameter(unit, cell, writeVals::pulseOnTime, testParameters.pont);
    emit updateParameter(unit, cell, writeVals::sinewaveFrequency,testParameters.sinewaveFrequency);
    emit updateParameter(unit, cell, writeVals::streamReportingPeriod, testParameters.reportingFrequency);

//    float hvc = 4.2f;
//    float lvc = 2.65f;
//    float hvcv = 4.2f;
//    float lvcv = 2.8f;
//    float htc = 45.0f;
//    float ltc = -20.0f;
//    float ccsc = 3.0f;
//    float dcsc = 3.0f;
//    float rf = 1.0f;
//    float ccs = 2.0f;
//    float sf = 1000.0f;
//    int pont = 600;
//    int poft = 600;
}
