#include "batlabcell.h"

batlabCell::batlabCell()
{

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
    int elapsed = timer.elapsed();

    if (mode != getCurrentMode()) {
    }

    if (status != 0x00) {
        emit emitError(id);
        return;
    }
    status = stat;
//    statusString = parseStatus(stat);
    time.append(elapsed);
    temperature.append(temp);
    current.append(curr);
    voltage.append(volt);
    modes.append(mode);

    if (volt > testParameters.hightVoltageCutoff || volt < testParameters.lowVoltageCutoff)
    {
        testPacket newTest;
        newTest.TIME = time;
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
        time.clear();

        voltagePP.clear();
        voltagePhase.clear();
        currentPhase.clear();
        currentPP.clear();

        testsToRun.removeFirst();

        emit testFinished(static_cast<int>(cell));
    }
}


batlabCell::~batlabCell()
{
}

testParms batlabCell::onGetParameters()
{
    return testParameters;
}

int batlabCell::onGetNextTest()
{
    if (!testsToRun.isEmpty()) {
        return testsToRun.first();
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
        currentPhase.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 360.0f / 256.0f)));
        break;
    case cellNamespace::CURRENT_PP:
        currentPP.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 4.096f / (pow(2,15) - 1.0f))));
        break;
    case cellNamespace::VOLTAGE_PHS:
        voltagePhase.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 360.0f / 256.0f)));
        break;
    case cellNamespace::VOLTAGE_PP:
        voltagePP.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 4.5f / (static_cast<float>(pow(2,15)) - 1.0f))));
        break;
    case cellNamespace::MODE:
        currentMode = value;
        break;
    default:
        break;
    }
}

void batlabCell::onUpdateParameters(int cell)
{
    emit updateParameter(cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_CHG), sendVoltageLimit(testParameters.hightVoltageCutoff));
    emit updateParameter(cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_DCHG), sendVoltageLimit(testParameters.lowVoltageCutoff));
    emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_CHG),  sendCurrentLimit(testParameters.currentCutoffCharge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_DCHG), sendCurrentLimit(testParameters.currentCutoffDischarge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::TEMP_LIMIT_CHG),  sendTemperatureLimits(testParameters.temperatureCutoffCharge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::TEMP_LIMIT_DCHG), sendTemperatureLimits(testParameters.temperatureCutoffDischarge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::REPORT_INTERVAL), sendReportingFrequency(testParameters.reportingFrequency));
    emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_SETPOINT), sendCurrentSetpoint(testParameters.currentSetpoint));
}
