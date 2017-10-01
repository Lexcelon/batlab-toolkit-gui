#ifndef GLOBALS
#define GLOBALS

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
    float currentCutoffCharge = 4.096f;
    float currentCutoffDischarge = 4.096f;
    float reportingFrequency = 1.0f;
    float chargeCurrentSetpoint = 2.0f;
    float dischargeCurrentSetpoint = 2.0f;
};

// Builtin variable default values and bounds

#define CELL_PLAYLIST_NAME_FIELDSTR                         "cellPlaylistName"
#define LIPO_CHEMISTRY_FIELDSTR                             "lipoChemistry"
#define IRON_PHOSPHATE_CHEMISTRY_FIELDSTR                   "ironPhosphateChemistry"
#define OTHER_CHEMISTRY_FIELDSTR                            "otherChemistry"

// Unit: Cells
#define NUM_CELLS_FIELDSTR                                  "numCells"
#define MINIMUM_NUM_CELLS                                   1
#define MAXIMUM_NUM_CELLS                                   1000
#define STARTING_CELL_NUMBER_FIELDSTR                       "startingCellNumber"
#define MINIMUM_STARTING_CELL_NUMBER                        0
#define MAXIMUM_STARTING_CELL_NUMBER                        100000
#define CELL_DESIGNATOR_FIELDSTR                            "cellDesignator"

// Unit: Volts
#define HIGH_VOLTAGE_CUTOFF_FIELDSTR                        "highVoltageCutoff"
#define HIGH_VOLTAGE_CUTOFF_DEFAULT                         4.2
#define HIGH_VOLTAGE_CUTOFF_MIN                             3.0
#define HIGH_VOLTAGE_CUTOFF_MAX                             4.4

#define IRON_PHOSPHATE_HIGH_VOLTAGE_CUTOFF_DEFAULT          3.6
#define IRON_PHOSPHATE_HIGH_VOLTAGE_CUTOFF_MAX              3.8

// Unit: Volts
#define LOW_VOLTAGE_CUTOFF_FIELDSTR                         "lowVoltageCutoff"
#define LOW_VOLTAGE_CUTOFF_DEFAULT                          2.5
#define LOW_VOLTAGE_CUTOFF_MIN                              2.0
#define LOW_VOLTAGE_CUTOFF_MAX                              3.6

#define IRON_PHOSPHATE_LOW_VOLTAGE_CUTOFF_DEFAULT           2.0

// Unit: Celsius
#define CHARGE_TEMP_CUTOFF_FIELDSTR                         "chargeTemperatureCutoff"
#define CHARGE_TEMP_CUTOFF_DEFAULT                          45.0
#define CHARGE_TEMP_CUTOFF_MIN                              25.0
#define CHARGE_TEMP_CUTOFF_MAX                              80.0

// Unit: Celsius
#define DISCHARGE_TEMP_CUTOFF_FIELDSTR                      "dischargeTemperatureCutoff"
#define DISCHARGE_TEMP_CUTOFF_DEFAULT                       65.0
#define DISCHARGE_TEMP_CUTOFF_MIN                           25.0
#define DISCHARGE_TEMP_CUTOFF_MAX                           80.0

// Unit: Amps
#define CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR               "chargeCurrentSafetyCutoff"
#define CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT                4.096
#define CHARGE_CURRENT_SAFETY_CUTOFF_MIN                    0.25
#define CHARGE_CURRENT_SAFETY_CUTOFF_MAX                    4.096

// Unit: Amps
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR            "dischargeCurrentSafetyCutoff"
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT             4.096
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN                 0.25
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX                 4.096

// Unit: Seconds
#define IMPEDANCE_REPORTING_PERIOD_FIELDSTR                 "impedanceReportingPeriod"
#define IMPEDANCE_REPORTING_PERIOD_DEFAULT                  300.0
#define IMPEDANCE_REPORTING_PERIOD_MIN                      10.0
#define IMPEDANCE_REPORTING_PERIOD_MAX                      3600.0

// Unit: Seconds
#define REPORTING_PERIOD_FIELDSTR                           "reportingPeriod"
#define REPORTING_PERIOD_DEFAULT                            1.0
#define REPORTING_PERIOD_MIN                                0.5
#define REPORTING_PERIOD_MAX                                3600.0

// Unit: Hz
#define SINE_WAVE_FREQUENCY_FIELDSTR                        "sineWaveFrequency"
#define SINE_WAVE_FREQUENCY_DEFAULT                         39.0625
#define SINE_WAVE_FREQUENCY_MIN                             39.0625
#define SINE_WAVE_FREQUENCY_MAX                             1054.6875

#define SINE_WAVE_MAGNITUDE_FIELDSTR                        "sineWaveMagnitude"
#define SINE_WAVE_MAGNITUDE_DEFAULT                         0
#define SINE_WAVE_MAGNITUDE_MIN                             0
#define SINE_WAVE_MAGNITUDE_MAX                             2

// Unit: Cycles
#define NUM_WARMUP_CYCLES_FIELDSTR                          "numWarmupCycles"
#define NUM_WARMUP_CYCLES_DEFAULT                           0
#define NUM_WARMUP_CYCLES_MIN                               0
#define NUM_WARMUP_CYCLES_MAX                               100

// Unit: Cycles
#define NUM_MEASUREMENT_CYCLES_FIELDSTR                     "numMeasurementCycles"
#define NUM_MEASUREMENT_CYCLES_DEFAULT                      1
#define NUM_MEASUREMENT_CYCLES_MIN                          0
#define NUM_MEASUREMENT_CYCLES_MAX                          10000

// Unit: Amps
#define CHARGE_RATE_FIELDSTR                                "chargeRate"
#define CHARGE_RATE_DEFAULT                                 2.0
#define CHARGE_RATE_MIN                                     0.25
// #define CHARGE_RATE_MAX                                  CHARGE_CURRENT_SAFETY_CUTOFF used

// Unit: Amps
#define DISCHARGE_RATE_FIELDSTR                             "dischargeRate"
#define DISCHARGE_RATE_DEFAULT                              2.0
#define DISCHARGE_RATE_MIN                                  0.25
// #define DISCHARGE_RATE_MAX                               DISCHARGE_CURRENT_SAFETY_CUTOFF used

// Unit: Boolean
#define STORAGE_DISCHARGE_FIELDSTR                          "storageDischarge"
#define STORAGE_DISCHARGE_DEFAULT                           true

// Unit: Volts
#define STORAGE_DISCHARGE_VOLTAGE_FIELDSTR                  "storageDischargeVoltage"
#define STORAGE_DISCHARGE_VOLTAGE_DEFAULT                   3.7
#define STORAGE_DISCHARGE_VOLTAGE_MIN                       2.0
// #define STORAGE_DISCHARGE_VOLTAGE_MAX                    HIGH_VOLTAGE_CUTOFF used

#define IRON_PHOSPHATE_STORAGE_DISCHARGE_VOLTAGE_DEFAULT    3.2

// Unit: Ohms
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR             "acceptableImpedanceThreshold"
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT              0.2
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN                  0.02
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX                  200.0

// Unit: Seconds
#define REST_PERIOD_FIELDSTR                                "restPeriod"
#define REST_PERIOD_DEFAULT                                 60.0
#define REST_PERIOD_MIN                                     0.0
#define REST_PERIOD_MAX                                     3600.0

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

enum cellNamespace {
    MODE,
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
    LOCK
};

enum unitNamespace {
    SERIAL_NUM,
    DEVICE_ID,
    FIRMWARE_VER,
    VCC,
    SINE_FREQ,
    SYSTEM_TIMER,
    SETTINGS,
    SINE_OFFSET,
    SINE_MAGDIV,
    LED_MESSAGE,
    BOOTLOAD,
    VOLT_CH_CALIB_OFF,
    VOLT_CH_CALIB_SCA,
    VOLT_DC_CALIB_OFF,
    VOLT_DC_CALIB_SCA

};

namespace bootloader
{
    enum {
        BOOTLOAD,
        ADDR,
        DATA
    };
}

enum commsNamespace {
    LED0,
    LED1,
    LED2,
    LED3,
    EXTERNAL_PSU,
    EXTERNAL_PSU_VOLTAGE
};

#endif // GLOBALS

