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
    sreamReportingPeriod,
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
    {sreamReportingPeriod, 0x50},
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

    void onGetTemp(int unit, int cell);
    void onGetCurr(int unit, int cell);
    void onGetCurrAmp(int unit, int cell);
    void onGetVol(int unit, int cell);
    void onGetVolPhase(int unit, int cell);
    void onGetVolAmp(int unit, int cell);
    void onGetEnergy(int unit, int cell);
    void onGetCharge(int unit, int cell);

    void onReadReg(int,int,vals);

private:
    QSerialPort * port;
    QVector<uchar> data;
};

#endif // BATLABCOM_H
