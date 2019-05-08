#ifndef BATLABLIB_H
#define BATLABLIB_H

#include <QDateTime>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QGenericMatrix>
#include <QLayout>
#include <QMap>
#include <QPushButton>
#include <QQueue>
#include <QSpinBox>
#include <QString>
#include <QThread>
#include <QVector>
#include <qmath.h>

#include "version.h"

extern QVector<QString> L_TEST_STATE;

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
  ZERO_AMP_THRESH,
  WATCHDOG_TIMER,
};

#define WATCHDOG_TIMER_RESET 255

enum bootloaderNamespace {
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

struct channelStatusInfo {
  int slot;

  QString cellName;

  bool testInProgress;
  float voltage_prev;
  float current_prev;

  bool preChargeComplete;
  bool preChargeError;

  int numWarmupCycles;
  int numWarmupCyclesCompleted;
  bool warmupCyclesError;

  int numMeasurementCycles;
  int numMeasurementCyclesCompleted;
  bool measurementCyclesError;

  bool storageDischarge;
  bool storageDischargeComplete;
  bool storageDischargeError;

  int tempCalibB;
  int tempCalibR;
};

struct batlabStatusInfo {
  bool confirmedBatlabDevice;
  bool inBootloader;
  bool externalPowerConnected;
  qint16 firmwareVersion;
  QString portName;
  qint16 serialNumberRegister;
  qint16 deviceIdRegister;
  qint32 serialNumberComplete;
  channelStatusInfo channels[4];
  qint32 firmwareBytesRemaining;
};

struct cellResultsStatusInfo {
  QString cellName;

  bool hasSomeResults;
  bool hasCompleteResults;

  bool testInProgress;
  int batlabSerial;
  int channel;

  float chargeCapacity;
  float chargeCapacityRange;
  float energyCapacity;
  float energyCapacityRange;
  float impedance;
  float avgCurrent;
  float avgVoltage;
};

struct batlabSettings {
  double highVoltageCutoff;
  double lowVoltageCutoff;
  double chargeTempCutoff;
  double dischargeTempCutoff;
  double chargeCurrentSafetyCutoff;
  double dischargeCurrentSafetyCutoff;
  double prechargeRate;
  double chargeRate;
  double dischargeRate;
  double sineWaveFrequency;
  int sineWaveMagnitude;

  bool enableConstantVoltage;
  double constantVoltageSensitivity;
  int constantVoltageStepSize;

  bool enableTrickle;
  double trickleDischargeEngageLimit;
  double trickleChargeEngageLimit;
  double trickleChargeRate;
  double trickleDischargeRate;

  bool enablePulse;
  double pulseDischargeOffTime;
  double pulseDischargeOnTime;
  double pulseChargeOffTime;
  double pulseChargeOnTime;
  double pulseChargeOffRate;
  double pulseDischargeOffRate;
};

#define DEFAULT_WRITE_TIMEOUT 500
#define DEFAULT_READ_TIMEOUT 2000
#define DEFAULT_SLEEP_AFTER_TRANSACTION 0
#define DEFAULT_BAUD_RATE 38400
#define DEFAULT_SERIAL_RETRIES 3

namespace BatlabLib {
cellResultsStatusInfo createInitializedCellResultsStatusInfo();
QString cellName(QString designator, int numCells, int startingCellNum,
                 int cellId);

static QMap<QString, int> namespaceStringToInt{
    {"Channel 0", batlabNamespaces::CHANNEL0},
    {"Channel 1", batlabNamespaces::CHANNEL1},
    {"Channel 2", batlabNamespaces::CHANNEL2},
    {"Channel 3", batlabNamespaces::CHANNEL3},
    {"Unit", batlabNamespaces::UNIT},
    {"Bootloader", batlabNamespaces::BOOTLOADER},
    {"Comms", batlabNamespaces::COMMS},
};

static QMap<int, QString> namespaceIntToString{
    {batlabNamespaces::CHANNEL0, "Channel 0"},
    {batlabNamespaces::CHANNEL1, "Channel 1"},
    {batlabNamespaces::CHANNEL2, "Channel 2"},
    {batlabNamespaces::CHANNEL3, "Channel 3"},
    {batlabNamespaces::UNIT, "Unit"},
    {batlabNamespaces::BOOTLOADER, "Bootloader"},
    {batlabNamespaces::COMMS, "Comms"},
};
} // namespace BatlabLib

// Status register codes
#define STAT_VOLTAGE_LIMIT_CHG 0x0001
#define STAT_VOLTAGE_LIMIT_DCHG 0x0002
#define STAT_CURRENT_LIMIT_CHG 0x0004
#define STAT_CURRENT_LIMIT_DCHG 0x0008
#define STAT_TEMP_LIMIT_CHG 0x0010
#define STAT_TEMP_LIMIT_DCHG 0x0020
#define STAT_BACKWARDS 0x0040
#define STAT_NO_CELL 0x0080
#define STAT_NO_PSU 0x0100
#define STAT_NOT_INITIALIZED 0x0200
#define STAT_NOT_CALIBRATED 0x0400

#define ERR_VOLTAGE_LIMIT_CHG 0x0001
#define ERR_VOLTAGE_LIMIT_DCHG 0x0002
#define ERR_CURRENT_LIMIT_CHG 0x0004
#define ERR_CURRENT_LIMIT_DCHG 0x0008
#define ERR_TEMP_LIMIT_CHG 0x0010
#define ERR_TEMP_LIMIT_DCHG 0x0020
#define ERR_BACKWARDS 0x0040
#define ERR_NO_CELL 0x0080
#define ERR_NO_PSU 0x0100
// ERR_LIST =
// ['ERR_VOLTAGE_LIMIT_CHG','ERR_VOLTAGE_LIMIT_DCHG','ERR_CURRENT_LIMIT_CHG','ERR_CURRENT_LIMIT_DCHG','ERR_TEMP_LIMIT_CHG','ERR_TEMP_LIMIT_DCHG','ERR_BACKWARDS','ERR_NO_CELL','ERR_NO_PSU','','','ERR_LOW_VCC']

#define SET_TRIM_OUTPUT 0x0001
#define SET_VCC_COMPENSATION 0x0002
#define SET_WATCHDOG_TIMER 0x0004
#define SET_CH0_HI_RES 0x0010
#define SET_NO_PSU_DCHG_ENABLE 0x0008
#define SET_DEBUG 0x8000

#define LOCK_LOCKED 0x0001
#define LOCK_UNLOCKED 0x0000

// Mode register codes
enum ChannelMode {
  MODE_NO_CELL = 0x0000,
  MODE_BACKWARDS,
  MODE_IDLE,
  MODE_CHARGE,
  MODE_DISCHARGE,
  MODE_IMPEDANCE,
  MODE_STOPPED
};
// MODE_LIST =
// ['MODE_NO_CELL','MODE_BACKWARDS','MODE_IDLE','MODE_CHARGE','MODE_DISCHARGE','MODE_IMPEDANCE','MODE_STOPPED']

// Test states
enum TestState {
  TS_IDLE = 0x0000,
  TS_CHARGE,
  TS_PRECHARGE,
  TS_DISCHARGE,
  TS_CHARGEREST,
  TS_DISCHARGEREST,
  TS_POSTDISCHARGE
};

// Firmware
#define FIRMWARE_FILE_SIZE 15360
#define MINIMUM_FIRMWARE_VERSION 4

#endif // BATLABLIB_H
