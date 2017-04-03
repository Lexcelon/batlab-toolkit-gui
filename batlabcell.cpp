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
        testsToRun.push_back(MODE_CHARGE);
    }

    for (int i = 0; i < cycles; i++) {
        testsToRun.push_back(MODE_DISCHARGE);
        testsToRun.push_back(MODE_CHARGE);
    }
}

void batlabCell::receiveStream(int mode, int stat, float temp, float curr, float volt)
{
    status = stat;
//    statusString = parseStatus(stat);
    temperature.append(temp);
    current.append(curr);
    voltage.append(volt);
    modes.append(mode);

    if (volt > testParameters.hightVoltageCutoff || volt < testParameters.lowVoltageCutoff)
    {
        testPacket newTest;
        newTest.REG_TEMPERATURE = temperature;
        newTest.REG_CURRENT = current;
        newTest.REG_VOLTAGE = voltage;
        newTest.REG_MODE = modes;
        tests.push_back(newTest);

        temperature.clear();
        current.clear();
        voltage.clear();
        modes.clear();

        //used for daisy chaining
        emit testFinished(static_cast<int>(cell), id, tests.size());
    }
}


batlabCell::~batlabCell()
{
}

//void batlabCell::receiveStream(int stat,float temp,int curr, int volt,int cha) {
//    status = stat;
//    statusString = parseStatus(stat);
//    temperature.append(temp);
//    current.append(curr);
//    voltage.append(volt);
//    charge.append(cha);

//    if (status & 0x01)
//    {
//        test newTest;
//        newTest.temperature = temperature;
//        newTest.current = current;
//        newTest.voltage = voltage;
//        newTest.charge = charge;
//        newTest.testType = testsToRun.first();
//        tests.push_back(newTest);

//        temperature.clear();
//        current.clear();
//        voltage.clear();
//        charge.clear();

//        //used for daisy chaining
////        emit testFinished((unit << 2)|cell);
//        emit testFinished(static_cast<int>(cell), id, tests.size());
//    }
//}


//void batlabCell::receiveStreamExt(int currAmp,int volPhase,int volAmp) {
//    currentAmplitude.append(currAmp);
//    voltagePhase.append(volPhase);
//    voltageAmplitude.append(volAmp);

//    if (status & 0x01) {
//        tests.last().currentAmplitude = currentAmplitude;
//        tests.last().voltagePhase = voltagePhase;
//        tests.last().voltageAmplitude = voltageAmplitude;

//        currentAmplitude.clear();
//        voltagePhase.clear();
//        voltageAmplitude.clear();
//    }
//}

void batlabCell::newTest(uchar testnum) {
//    test newTest;
//    newTest.mode = testnum;
//    tests.push_back(newTest);
}


testParms batlabCell::onGetParameters()
{
    return testParameters;
}

int batlabCell::onGetNextTest()
{
    if (!testsToRun.isEmpty()) {
        return testsToRun.takeFirst();
    } else {
        return -1;
    }
}

void batlabCell::setSineFreq(float freq)
{
    sineFreq = freq;
}

void batlabCell::onUpdateParameters(int unit, int cell)
{
    emit updateParameter(0xAA, cell, cellNamespace::VOLTAGE_LIMIT_CHG, sendVoltageLimit(testParameters.hightVoltageCutoff));
    emit updateParameter(0xAA, cell, cellNamespace::VOLTAGE_LIMIT_DCHG, sendVoltageLimit(testParameters.lowVoltageCutoff));
    emit updateParameter(0xAA, cell, cellNamespace::CURRENT_LIMIT_CHG,  sendCurrentLimit(testParameters.currentCutoffCharge));
    emit updateParameter(0xAA, cell, cellNamespace::CURRENT_LIMIT_DCHG, sendCurrentLimit(testParameters.currentCutoffDischarge));
    emit updateParameter(0xAA, cell, cellNamespace::TEMP_LIMIT_CHG,  sendTemperatureLimits(testParameters.temperatureCutoffCharge));
    emit updateParameter(0xAA, cell, cellNamespace::TEMP_LIMIT_DCHG, sendTemperatureLimits(testParameters.temperatureCutoffDischarge));
    emit updateParameter(0xAA, cell, cellNamespace::REPORT_INTERVAL, sendReportingFrequency(testParameters.reportingFrequency));
    emit updateParameter(0xAA, cell, cellNamespace::CURRENT_SETPOINT, sendCurrentSetpoint(testParameters.currentSetpoint));
}
