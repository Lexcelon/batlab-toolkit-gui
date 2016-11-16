#ifndef GLOBALS
#define GLOBALS
#include <QMap>
#include <QString>
#include <qmath.h>
#include <QVector>


enum vals {
    status,
    mode,
    temperature,
    current,
    currentAmplitude,
    voltage,
    voltagePhase,
    voltageAmplitude,
    energy,
    charge,
    numberOfConnectedUnits,
    numberOfConnectedCells,
    chargeVoltageCutoff,
    dischargeVoltageCutoff,
    chargeCVVoltageCutoff,
    dischargeCVVoltageCutoff,
    chargeCurrentCutoff,
    dischargeCurrentCutoff,
    chargeCVCurrentCutoff,
    dischargeCVCurrentCutoff,
    sineChargeCurrentIppSetpoint,
    sineDischargeCurrentIppSetpoint,
    highTempChargeSafetyCutoff,
    highTempDischargeSafetyCutoff,
    chargeCurrentSafetyCutoff,
    dischargeCurrentSafetyCutoff,
    streamReportingPeriod,
    sinewaveFrequency,
    pulseOnTime,
    pulseOffTime,
    NC_VOLTAGE,
    SYSTEM_TIMER,
    STREAM_TIMER,
    CV_DUTY,
    PULSE_TIMER,
    phase_timestamp_highRes,
    phase_timestamp_lowRes,
    command,
    unitUniqueSerialID

};

static QMap<vals, uchar> registers = {
    {status, 0x00 },
    {mode, 0x01 },
    {temperature, 0x02 },
    {current, 0x03},
    {currentAmplitude, 0x04},
    {voltage, 0x05},
    {voltagePhase, 0x06},
    {voltageAmplitude, 0x07},
    {energy, 0x08},
    {charge, 0x09},
    {numberOfConnectedUnits, 0x40},
    {numberOfConnectedCells, 0x41},
    {chargeVoltageCutoff, 0x42},
    {dischargeVoltageCutoff, 0x43},
    {chargeCVVoltageCutoff, 0x44},
    {dischargeCVVoltageCutoff, 0x45},
    {chargeCurrentCutoff, 0x46},
    {dischargeCurrentCutoff, 0x47},
    {chargeCVCurrentCutoff, 0x48},
    {dischargeCVCurrentCutoff, 0x49},
    {sineChargeCurrentIppSetpoint, 0x4A},
    {sineDischargeCurrentIppSetpoint, 0x4B},
    {highTempChargeSafetyCutoff, 0x4C},
    {highTempDischargeSafetyCutoff, 0x4D},
    {chargeCurrentSafetyCutoff, 0x4E},
    {dischargeCurrentSafetyCutoff, 0x4F},
    {streamReportingPeriod, 0x50},
    {sinewaveFrequency, 0x51},
    {pulseOnTime, 0x52},
    {pulseOffTime, 0x53},
    {NC_VOLTAGE, 0x54},
    {SYSTEM_TIMER, 0x55},
    {STREAM_TIMER, 0x56},
    {CV_DUTY, 0x0B},
    {PULSE_TIMER, 0x0C},
    {phase_timestamp_highRes, 0x0D},
    {phase_timestamp_lowRes, 0x0E},
    {command, 0x60},
    {unitUniqueSerialID, 0x61}
};


static QMap<uchar, vals> enumVals = {
    { 0x00, status },
    { 0x01, mode },
    { 0x02, temperature },
    { 0x03, current},
    { 0x04, currentAmplitude},
    { 0x05, voltage},
    { 0x06, voltagePhase},
    { 0x07, voltageAmplitude,},
    { 0x08, energy},
    { 0x09, charge},
    { 0x40, numberOfConnectedUnits},
    { 0x41, numberOfConnectedCells},
    { 0x42, chargeVoltageCutoff},
    { 0x43, dischargeVoltageCutoff},
    { 0x44, chargeCVVoltageCutoff},
    { 0x45, dischargeCVVoltageCutoff},
    { 0x46, chargeCurrentCutoff},
    { 0x47, dischargeCurrentCutoff},
    { 0x48, chargeCVCurrentCutoff},
    { 0x49, dischargeCVCurrentCutoff},
    { 0x4A, sineChargeCurrentIppSetpoint},
    { 0x4B, sineDischargeCurrentIppSetpoint},
    { 0x4C, highTempChargeSafetyCutoff},
    { 0x4D, highTempDischargeSafetyCutoff},
    { 0x4E, chargeCurrentSafetyCutoff},
    { 0x4F, dischargeCurrentSafetyCutoff},
    { 0x50, streamReportingPeriod},
    { 0x51, sinewaveFrequency},
    { 0x52, pulseOnTime},
    { 0x53, pulseOffTime},
    { 0x54, NC_VOLTAGE},
    { 0x55, SYSTEM_TIMER},
    { 0x56, STREAM_TIMER},
    { 0x0B, CV_DUTY},
    { 0x0C, PULSE_TIMER},
    { 0x0D, phase_timestamp_highRes},
    { 0x0E, phase_timestamp_lowRes},
    { 0x60, command},
    { 0x61, unitUniqueSerialID}
};

static QMap<int, QString> statusString = {
    { 0x0001, "DONE" },
    { 0x0002, "IDLE" },
    { 0x0004, "NOT_CONNECTED" },
    { 0x0008, "BACKWARDS" },
    { 0x0010, "OVERV" },
    { 0x0020, "UNDERV" },
    { 0x0040, "OVERT" },
    { 0x0080, "OVERI" },
    { 0x0100, "STREAM_PKT" },
    { 0x0200, "EXT_STREAM_PKT" },
    { 0x0400, "CC_CV_STEP2" },
    { 0x0800, "PULSE_ACTIVE" },
    { 0x1000, "RLY_CHG" }
};

static QMap<vals, QString> names = {
    {status, "Status" },
    {mode, "Mode" },
    {temperature, "Temperature" },
    {current, "Current"},
    {currentAmplitude, "Current Amplitude"},
    {voltage, "Voltage"},
    {voltagePhase, "Voltage Phase"},
    {voltageAmplitude, "Voltage Amplitude"},
    {energy, "Energy"},
    {charge, "Charge"},
    {numberOfConnectedUnits, "Number of Connected Units"},
    {numberOfConnectedCells, "Number of Connected Cells"},
    {chargeVoltageCutoff, "Charge Voltage Cutoff"},
    {dischargeVoltageCutoff, "Discharge Voltage Cutoff"},
    {chargeCVVoltageCutoff, "Charge CV Voltage Cutoff"},
    {dischargeCVVoltageCutoff, "Discharge CV Voltage Cutoff"},
    {chargeCurrentCutoff, "Charge Current Cutoff"},
    {dischargeCurrentCutoff, "Discharge Current Cutoff"},
    {chargeCVCurrentCutoff, "Charge CV Current Cutoff"},
    {dischargeCVCurrentCutoff, "Discharge CV Current Cutoff"},
    {sineChargeCurrentIppSetpoint, "Sine Charge Current IPP Setpoint"},
    {sineDischargeCurrentIppSetpoint, "Sine Discharge Current IPP Setpoint"},
    {highTempChargeSafetyCutoff, "High Temperature Charge Safety Cutoff"},
    {highTempDischargeSafetyCutoff, "High Temperature Discharge Safety Cutoff"},
    {chargeCurrentSafetyCutoff, "Charge Current Safety Cutoff"},
    {dischargeCurrentSafetyCutoff, "Discharge Current Safety Cutoff"},
    {streamReportingPeriod, "Stream Reporting Period"},
    {sinewaveFrequency, "Sinewave Frequency"},
    {pulseOnTime, "Pulse On Time"},
    {pulseOffTime, "Pulse Off Time"},
    {NC_VOLTAGE, "NC_VOLTAGE"},
    {SYSTEM_TIMER, "SYSTEM_TIMER"},
    {STREAM_TIMER, "STREAM_TIMER"},
    {CV_DUTY, "CV_DUTY"},
    {PULSE_TIMER, "PULSE_TIMER"},
    {phase_timestamp_highRes, "phase_timestamp_high res"},
    {phase_timestamp_lowRes, "phase_timestamp_low res"},
    {command, "Command"},
    {unitUniqueSerialID, "Unit Unique Serial ID"}

};

enum modeCodes {
    idle,
    ccCharge,
    cccvCharge,
    pulsedCharge,
    sineCharge,
    ccDischarge,
    cccvDischarge,
    pulsedDischarge,
    sineDischarge
};

static QMap<modeCodes,uchar> modeRegCodes = {
  {idle, 0x00},
    {ccCharge, 0x01},
    {cccvCharge, 0x02},
    {pulsedCharge, 0x03},
    {sineCharge, 0x04},
    {ccDischarge, 0x05},
    {cccvDischarge, 0x06},
    {pulsedDischarge, 0x07},
    {sineDischarge, 0x08}
};

static QMap<uchar,modeCodes> enumModeCodes = {
    { 0x00, idle},
    { 0x01, ccCharge},
    { 0x02, cccvCharge},
    { 0x03, pulsedCharge},
    { 0x04, sineCharge},
    { 0x05, ccDischarge},
    { 0x06, cccvDischarge},
    { 0x07, pulsedDischarge},
    { 0x08, sineDischarge}
};

static QMap<modeCodes,QString> modeRegCodeNames = {
  {idle, "IDLE"},
    {ccCharge, "CC CHARGE"},
    {cccvCharge, "CC-CV CHARGE"},
    {pulsedCharge, "PULSED CHARGE"},
    {sineCharge, "SINE CHARGE"},
    {ccDischarge, "CC DISCHARGE"},
    {cccvDischarge, "CC-CV DISCHARGE"},
    {pulsedDischarge, "PULSED DISCHARGE"},
    {sineDischarge, "SINE DISCHARGE"}
};


enum commandCodes {
    start,
    stop,
    pause,
    resume
};

static QMap<commandCodes,uchar> commandRegCodes = {
    {start, 0x01},
    {stop, 0x02},
    {pause, 0x03},
    {resume, 0x04}
};

static QMap<uchar,commandCodes> enumCommandCodes = {
    { 0x01, start},
    { 0x02, stop},
    { 0x03, pause},
    { 0x04, resume}
};

static QMap<commandCodes,QString> commandRegCodeNames = {
    {start, "START"},
    {stop, "STOP"},
    {pause, "PAUSE"},
    {resume, "RESUME"}
};


enum class writeVals {
    mode,
    chargeVoltageCutoff,
    dischargeVoltageCutoff,
    chargeCVVoltageCutoff,
    dischargeCVVoltageCutoff,
    chargeCurrentCutoff,
    dischargeCurrentCutoff,
    chargeCVCurrentCutoff,
    dischargeCVCurrentCutoff,
    sineChargeCurrentIppSetpoint,
    sineDischargeCurrentIppSetpoint,
    highTempChargeSafetyCutoff,
    highTempDischargeSafetyCutoff,
    chargeCurrentSafetyCutoff,
    dischargeCurrentSafetyCutoff,
    streamReportingPeriod,
    sinewaveFrequency,
    pulseOnTime,
    pulseOffTime,
    NC_VOLTAGE,
    command
};

static QMap<writeVals, uchar> writeRegisters = {
    {writeVals::mode, 0x01 },
    {writeVals::chargeVoltageCutoff, 0x42},
    {writeVals::dischargeVoltageCutoff, 0x43},
    {writeVals::chargeCVVoltageCutoff, 0x44},
    {writeVals::dischargeCVVoltageCutoff, 0x45},
    {writeVals::chargeCurrentCutoff, 0x46},
    {writeVals::dischargeCurrentCutoff, 0x47},
    {writeVals::chargeCVCurrentCutoff, 0x48},
    {writeVals::dischargeCVCurrentCutoff, 0x49},
    {writeVals::sineChargeCurrentIppSetpoint, 0x4A},
    {writeVals::sineDischargeCurrentIppSetpoint, 0x4B},
    {writeVals::highTempChargeSafetyCutoff, 0x4C},
    {writeVals::highTempDischargeSafetyCutoff, 0x4D},
    {writeVals::chargeCurrentSafetyCutoff, 0x4E},
    {writeVals::dischargeCurrentSafetyCutoff, 0x4F},
    {writeVals::streamReportingPeriod, 0x50},
    {writeVals::sinewaveFrequency, 0x51},
    {writeVals::pulseOnTime, 0x52},
    {writeVals::pulseOffTime, 0x53},
    {writeVals::NC_VOLTAGE, 0x54},
    {writeVals::command, 0x60},
};


static QMap<uchar, writeVals> enumWriteVals = {
    { 0x01, writeVals::mode },
    { 0x42, writeVals::chargeVoltageCutoff},
    { 0x43, writeVals::dischargeVoltageCutoff},
    { 0x44, writeVals::chargeCVVoltageCutoff},
    { 0x45, writeVals::dischargeCVVoltageCutoff},
    { 0x46, writeVals::chargeCurrentCutoff},
    { 0x47, writeVals::dischargeCurrentCutoff},
    { 0x48, writeVals::chargeCVCurrentCutoff},
    { 0x49, writeVals::dischargeCVCurrentCutoff},
    { 0x4A, writeVals::sineChargeCurrentIppSetpoint},
    { 0x4B, writeVals::sineDischargeCurrentIppSetpoint},
    { 0x4C, writeVals::highTempChargeSafetyCutoff},
    { 0x4D, writeVals::highTempDischargeSafetyCutoff},
    { 0x4E, writeVals::chargeCurrentSafetyCutoff},
    { 0x4F, writeVals::dischargeCurrentSafetyCutoff},
    { 0x50, writeVals::streamReportingPeriod},
    { 0x51, writeVals::sinewaveFrequency},
    { 0x52, writeVals::pulseOnTime},
    { 0x53, writeVals::pulseOffTime},
    { 0x54, writeVals::NC_VOLTAGE},
    { 0x60, writeVals::command},
};

static QMap<writeVals, QString> writeNames = {
    { writeVals::mode, "Mode" },
    { writeVals::chargeVoltageCutoff, "Charge Voltage Cutoff"},
    { writeVals::dischargeVoltageCutoff, "Discharge Voltage Cutoff"},
    { writeVals::chargeCVVoltageCutoff, "Charge CV Voltage Cutoff"},
    { writeVals::dischargeCVVoltageCutoff, "Discharge CV Voltage Cutoff"},
    { writeVals::chargeCurrentCutoff, "Charge Current Cutoff"},
    { writeVals::dischargeCurrentCutoff, "Discharge Current Cutoff"},
    { writeVals::chargeCVCurrentCutoff, "Charge CV Current Cutoff"},
    { writeVals::dischargeCVCurrentCutoff, "Discharge CV Current Cutoff"},
    { writeVals::sineChargeCurrentIppSetpoint, "Sine Charge Current IPP Setpoint"},
    { writeVals::sineDischargeCurrentIppSetpoint, "Sine Discharge Current IPP Setpoint"},
    { writeVals::highTempChargeSafetyCutoff, "High Temperature Charge Safety Cutoff"},
    { writeVals::highTempDischargeSafetyCutoff, "High Temperature Discharge Safety Cutoff"},
    { writeVals::chargeCurrentSafetyCutoff, "Charge Current Safety Cutoff"},
    { writeVals::dischargeCurrentSafetyCutoff, "Discharge Current Safety Cutoff"},
    { writeVals::streamReportingPeriod, "Stream Reporting Period"},
    { writeVals::sinewaveFrequency, "Sinewave Frequency"},
    { writeVals::pulseOnTime, "Pulse On Time"},
    { writeVals::pulseOffTime, "Pulse Off Time"},
    { writeVals::NC_VOLTAGE, "NC_VOLTAGE"},
    { writeVals::command, "Command"}
};

static QString parseStatus(int status) {
    QString str;
    for (int i = 0; i < statusString.size(); ++i) {
        if (status & (1<<i)) {
                str = str + " " + statusString[1<<i];
        }
    }
    return str;
}

static float getTemp(int val) {
    float R = 10000/ ((pow(2,15)/float(val))-1);
    float To = 25.0f + 273.15f;
    float Ro = 10000;
    float B = 3428;
    float Tinv = (1/To) + (log(R/Ro)/B);
    float T = (1/Tinv) - 273.15f;
    T = (T * 1.8f) +32;
    return T;
}


enum class properties {
    unit,
    cell,
    status,
    statusString,
    temperature,
    current,
    voltage,
    charge,
    currentAmplitude,
    voltagePhase,
    voltageAmplitude
};


static QMap<properties,QString> cellProperties {
    { properties::unit, "Unit" },
    { properties::cell, "Cell" },
    { properties::status, "Status" },
    { properties::statusString, "Status String" },
    { properties::temperature, "Temperature" },
    { properties::current, "Current" },
    { properties::voltage, "Voltage" },
    { properties::charge, "Charge" },
    { properties::currentAmplitude, "Current Amplitude" },
    { properties::voltagePhase, "Voltage Phase" },
    { properties::voltageAmplitude, "Voltage Amplitude" }
};
#endif // GLOBALS

