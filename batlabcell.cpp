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
    qDebug() << Q_FUNC_INFO << mode << stat << temp << curr << volt;
    int elapsed = timer.elapsed();

    if (mode != getCurrentMode()) {
    }

    if (stat != 0x00) {
        emit emitError(id);
        return;
    }
    status = stat;
    time.append(elapsed);
    temperature.append(temp);
    current.append(curr);
    voltage.append(volt);
    modes.append(mode);

    if (((volt > testParameters.hightVoltageCutoff - 0.05f) && (mode == MODE_CHARGE)) || ((volt < testParameters.lowVoltageCutoff + 0.05f) && (mode == MODE_DISCHARGE)))
    {        
        emit testFinished(static_cast<int>(cell));
        emit updateUI(id, tests.size());
        testPacket newTest;

        for (int i = time.size() - 1; i >= 0; --i) {
            time[i] -= time[0];
        }
        newTest.TIME = time;
        newTest.REG_TEMPERATURE = temperature;
        newTest.REG_CURRENT = current;
        newTest.REG_VOLTAGE = voltage;
        newTest.REG_MODE = modes;
        newTest.VOLTAGE_PP = voltagePP;
        newTest.CURRENT_PP = currentPP;
        for (int i = 0; (i < chargeH.size()) && (i < chargeL.size()); i++) {
            newTest.CHARGE.push_back(QPair<int,int>(chargeL[i].first, chargeL[i].second + (chargeH[i].second << 16)));
        }
        tests.push_back(newTest);

        temperature.clear();
        current.clear();
        voltage.clear();
        modes.clear();
        time.clear();

        voltagePP.clear();
        currentPP.clear();



        if (!testsToRun.isEmpty()) {
            testsToRun.removeFirst();
        }

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
    qDebug() << Q_FUNC_INFO << batlabRegister << value;
    switch (batlabRegister) {
    case cellNamespace::CURRENT_PP:
        currentPP.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 4.096f / (pow(2,15) - 1.0f))));
        break;
    case cellNamespace::VOLTAGE_PP:
        voltagePP.push_back(QPair<float,QPair<int,float>>(sineFreq, QPair<int,float>(timer.elapsed(),static_cast<float>(value) * 4.5f / (static_cast<float>(pow(2,15)) - 1.0f))));
        break;
    case cellNamespace::CHARGEH:
        chargeH.push_back(QPair<int,int>(timer.elapsed(), value));
        if (chargeH.size() > 0 && chargeL.size() > 0) {
            tests.last().CHARGE.push_back(QPair<int,int>(timer.elapsed(), chargeL[0].second + (chargeH[0].second << 16)));
            chargeL.clear();
            chargeH.clear();
        }
        break;
    case cellNamespace::CHARGEL:
        chargeL.push_back(QPair<int,int>(timer.elapsed(), value));
        if (chargeH.size() > 0 && chargeL.size() > 0) {
            tests.last().CHARGE.push_back(QPair<int,int>(timer.elapsed(), chargeL[0].second + (chargeH[0].second << 16)));
            chargeL.clear();
            chargeH.clear();
        }
        break;
    case cellNamespace::MODE:
        currentMode = value;
        break;
    default:
        break;
    }
}

void batlabCell::onUpdateParameters(int cells)
{
    cell = cells;
    qDebug() << "Parameter: " << (testParameters.hightVoltageCutoff);
    qDebug() << "Parameter: " << (testParameters.lowVoltageCutoff);
//    qDebug() << "Parameter: " << (testParameters.currentCutoffCharge);
//    qDebug() << "Parameter: " << (testParameters.currentCutoffDischarge);
//    qDebug() << "Parameter: " << (testParameters.temperatureCutoffCharge);
//    qDebug() << "Parameter: " << (testParameters.temperatureCutoffDischarge);
//    qDebug() << "Parameter: " << (testParameters.reportingFrequency);
//    qDebug() << "Parameter: " << (testParameters.currentSetpoint);
    emit updateParameter(cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_CHG), sendVoltageLimit(testParameters.hightVoltageCutoff));
    emit updateParameter(cell, static_cast<int>(cellNamespace::VOLTAGE_LIMIT_DCHG), sendVoltageLimit(testParameters.lowVoltageCutoff));
    emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_CHG),  sendCurrentLimit(testParameters.currentCutoffCharge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_LIMIT_DCHG), sendCurrentLimit(testParameters.currentCutoffDischarge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::TEMP_LIMIT_CHG),  sendTemperatureLimits(testParameters.temperatureCutoffCharge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::TEMP_LIMIT_DCHG), sendTemperatureLimits(testParameters.temperatureCutoffDischarge));
    emit updateParameter(cell, static_cast<int>(cellNamespace::REPORT_INTERVAL), sendReportingFrequency(testParameters.reportingFrequency));

    if (onGetNextTest() == MODE_CHARGE) {
        emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_SETPOINT), sendCurrentSetpoint(testParameters.chargeCurrentSetpoint));
    } else if (onGetNextTest() == MODE_DISCHARGE){
        emit updateParameter(cell, static_cast<int>(cellNamespace::CURRENT_SETPOINT), sendCurrentSetpoint(testParameters.dischargeCurrentSetpoint));
    }
}
