#ifndef BATLABLIB_H
#define BATLABLIB_H

#include <QMap>
#include <QString>
#include <qmath.h>
#include <QThread>
#include <QGenericMatrix>
#include <QVector>
#include <QLayout>
#include <QDebug>
#include <QPushButton>
#include <QSpinBox>
#include <QDateTime>
#include <QDoubleSpinBox>

#include "version.h"

namespace BatlabLib
{
    void bubblesort(QVector<float> data, QVector<int> &indices);
    float getCharge(int val);
    float getTemp(int val);
    float getTemp(int val, int tempCalibB, int tempCalibR);
    float getTemperature(signed short val);
    float getCurrent(signed short val);
    float getVoltage(signed short val);
    float getPhase(unsigned char val);
    int sendTemperatureLimits();
    int sendCurrentLimit(float val);
    int sendVoltageLimit(float val);
    int sendReportingFrequency(float val);
    int sendCurrentSetpoint(float val);
    float getSineFrequency(int val);
    int sendSineFrequency(float val);
}

struct testParms
{
    int restTime = 10; //300;
    float hightVoltageCutoff = 4.2f;
    float lowVoltageCutoff = 2.65f;
    float temperatureCutoffCharge = 45.0f;
    float temperatureCutoffDischarge = 0.0f;
    float currentCutoffCharge = 4.095f;
    float currentCutoffDischarge = 4.095f;
    float reportingFrequency = 1.0f;
    float chargeCurrentSetpoint = 2.0f;
    float dischargeCurrentSetpoint = 2.0f;
};

//Status register codes
#define STAT_VOLTAGE_LIMIT_CHG  0x0001
#define STAT_VOLTAGE_LIMIT_DCHG 0x0002
#define STAT_CURRENT_LIMIT_CHG  0x0004
#define STAT_CURRENT_LIMIT_DCHG 0x0008
#define STAT_TEMP_LIMIT_CHG     0x0010
#define STAT_TEMP_LIMIT_DCHG    0x0020
#define STAT_BACKWARDS          0x0040
#define STAT_NO_CELL            0x0080
#define STAT_NO_PSU             0x0100
#define STAT_NOT_INITIALIZED    0x0200
#define STAT_NOT_CALIBRATED     0x0400

#define ERR_VOLTAGE_LIMIT_CHG  0x0001
#define ERR_VOLTAGE_LIMIT_DCHG 0x0002
#define ERR_CURRENT_LIMIT_CHG  0x0004
#define ERR_CURRENT_LIMIT_DCHG 0x0008
#define ERR_TEMP_LIMIT_CHG     0x0010
#define ERR_TEMP_LIMIT_DCHG    0x0020
#define ERR_BACKWARDS          0x0040
#define ERR_NO_CELL            0x0080
#define ERR_NO_PSU             0x0100

#define SET_TRIM_OUTPUT        0x0001
#define SET_VCC_COMPENSATION   0x0002
#define SET_DEBUG              0x8000

//Mode register codes
#define MODE_NO_CELL           0x0000
#define MODE_BACKWARDS         0x0001
#define MODE_IDLE              0x0002
#define MODE_CHARGE            0x0003
#define MODE_DISCHARGE         0x0004
#define MODE_IMPEDANCE         0x0005
#define MODE_STOPPED           0x0006

struct testPacket {
    QDateTime time;
    QVector<int> TIME;
    QVector<int> REG_MODE;
    QVector<int> REG_STATUS;
    QVector<float> REG_TEMPERATURE;
    QVector<float> REG_CURRENT;
    QVector<float> REG_VOLTAGE;
    QVector<QPair<float,QPair<int,float>>> VOLTAGE_PP;
    QVector<QPair<float,QPair<int,float>>> CURRENT_PP;
    QVector<QPair<int,int>> CHARGE;
};

struct channelInfo {
    QString cellName;

    bool testInProgress;

    bool preChargeComplete;

    int numWarmupCycles;
    int numWarmupCyclesCompleted;

    int numMeasurementCycles;
    int numMeasurementCyclesCompleted;

    bool storageDischarge;
    bool storageDischargeComplete;
};

struct batlabInfo {
    bool externalPowerConnected;
    qint16 firmwareVersion;
    QString portName;
    qint16 serialNumber;
    qint16 deviceId;
    channelInfo channels[4];
};

enum batlabNamespaces {
    CHANNEL0 = 0x00,
    CHANNEL1 = 0x01,
    CHANNEL2 = 0x02,
    CHANNEL3 = 0x03,
    UNIT = 0x04,
    BOOTLOADER = 0x05,
    COMMS = 0xFF,
};

enum cellNamespace {
    MODE = 0x00,
    CELL_ERROR,
    STATUS,
    CURRENT_SETPOINT,
    REPORT_INTERVAL,

    TEMPERATURE,
    CURRENT,
    VOLTAGE,
    CHARGEL,
    CHARGEH,

    VOLTAGE_LIMIT_CHG,
    VOLTAGE_LIMIT_DCHG,
    CURRENT_LIMIT_CHG,
    CURRENT_LIMIT_DCHG,
    TEMP_LIMIT_CHG,
    TEMP_LIMIT_DCHG,

    DUTY,
    COMPENSATION,
    CURRENT_PP,
    VOLTAGE_PP,

    CURRENT_CALIB_OFF,
    CURRENT_CALIB_SCA,
    TEMP_CALIB_R,
    TEMP_CALIB_B,
    CURRENT_CALIB_PP,
    VOLTAGE_CALIB_PP,
    CURR_CALIB_PP_OFF,
    VOLT_CALIB_PP_OFF,
};

enum unitNamespace {
    SERIAL_NUM = 0x00,
    DEVICE_ID,
    FIRMWARE_VER,
    VCC,
    SINE_FREQ,
    SYSTEM_TIMER,
    SETTINGS,
    SINE_OFFSET,
    SINE_MAGDIV,
    LED_MESSAGE,
    UNIT_BOOTLOAD,
    VOLT_CH_CALIB_OFF,
    VOLT_CH_CALIB_SCA,
    VOLT_DC_CALIB_OFF,
    VOLT_DC_CALIB_SCA,
    LOCK,
};

enum bootloaderNamespace
{
    BOOTLOAD = 0x00,
    ADDR,
    DATA,
};

enum commsNamespace {
    LED0 = 0x00,
    LED1,
    LED2,
    LED3,
    EXTERNAL_PSU,
    EXTERNAL_PSU_VOLTAGE,
};

#endif // BATLABLIB_H

