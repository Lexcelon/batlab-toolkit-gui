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
        newTest.VOLTAGE_PP = voltagePP;
        newTest.VOLTAGE_PHASE = voltagePhase;
        newTest.CURRENT_PHASE = currentPhase;
        newTest.CURRENT_PP = currentPP;
        tests.push_back(newTest);

        temperature.clear();
        current.clear();
        voltage.clear();
        modes.clear();

        voltagePP.clear();
        voltagePhase.clear();
        currentPhase.clear();
        currentPP.clear();

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

void batlabCell::receiveReadResponse(int batlabRegister, int value)
{
    switch (batlabRegister) {
    case cellNamespace::CURRENT_PHS:
        currentPhase.push_back(QPair<float,float>(sineFreq, static_cast<float>(value) * 360.0f / 256.0f));
        break;
    case cellNamespace::CURRENT_PP:
        currentPP.push_back(QPair<float,float>(sineFreq, static_cast<float>(value) * 4.096f / (pow(2,15) - 1.0f)));
        break;
    case cellNamespace::VOLTAGE_PHS:
        voltagePhase.push_back(QPair<float,float>(sineFreq, static_cast<float>(value) * 360.0f / 256.0f));
        break;
    case cellNamespace::VOLTAGE_PP:
        voltagePP.push_back(QPair<float,float>(sineFreq, static_cast<float>(value) * 4.5f / (static_cast<float>(pow(2,15)) - 1.0f)));
        break;
    default:
        break;
    }
}

void batlabCell::onUpdateParameters(int cell)
{
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_CHG), sendVoltageLimit(testParameters.hightVoltageCutoff));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_DCHG), sendVoltageLimit(testParameters.lowVoltageCutoff));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_CHG),  sendCurrentLimit(testParameters.currentCutoffCharge));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_DCHG), sendCurrentLimit(testParameters.currentCutoffDischarge));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::TEMP_LIMIT_CHG),  sendTemperatureLimits(testParameters.temperatureCutoffCharge));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::TEMP_LIMIT_DCHG), sendTemperatureLimits(testParameters.temperatureCutoffDischarge));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::REPORT_INTERVAL), sendReportingFrequency(testParameters.reportingFrequency));
    emit updateParameter(0xAA, cell, static_cast<int>(cellNamespace::CURRENT_SETPOINT), sendCurrentSetpoint(testParameters.currentSetpoint));
}
