#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVector>
#include <QDebug>
#include <iostream>
#include <map>


enum vals {
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
    phase_timestamp_lowRes

};

static std::map<vals, uchar> registers = {
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
    {phase_timestamp_lowRes, 0x0E}
};


static std::map<uchar, vals> enumVals = {
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
    { 0x0E, phase_timestamp_lowRes}
};


static std::map<vals, QString> names = {
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
    {phase_timestamp_lowRes, "phase_timestamp_low res"}

};

class batlabCom : public QObject
{
    Q_OBJECT
public:
    explicit batlabCom(QObject *parent = 0);
    ~batlabCom();

signals:
    void emitResponse(int,int,QString,int);
    void emitStream(int,int,int,int,int,int,int);
    void emitStreamExt(int,int,int,int,int);


public slots:
    void onRead();
    void parseData();
    void onReadReg(int,int,vals);

private:
    QSerialPort * port;
    QVector<uchar> data;
};

#endif // BATLABCOM_H
