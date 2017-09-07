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


static void bubblesort(QVector<float> data, QVector<int> &indices)
{
    for (int i = 0; i < data.size() - 1; ++i) {
        for (int j = 0; j < data.size() - 1 - i ; ++j) {
            if (data.at(j) > data.at(j+1)) {
                float swap = data[j];
                data[j] = data[j+1];
                data[j+1] = swap;

                swap = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = swap;
            }
        }
    }
}

static float getCharge(int val) {
    return (6. * (static_cast<float>(val) / pow(2,15) )) * 4.096 / 9.765625;
}

static float getTemp(int val)
{
    float R = 10000/ ((pow(2,15)/float(val))-1);
    float To = 25.0f + 273.15f;
    float Ro = 10000;
    float B = 3428;
    float Tinv = (1/To) + (log(R/Ro)/B);
    float T = (1/Tinv) - 273.15f;
    T = (T * 1.8f) +32;
    return T;
}


static float getTemp(int val, int tempCalibB, int tempCalibR)
{
    float Rtherm = static_cast<float>(tempCalibR) * 4.096 / (((pow(2,15) - 1)/ val) - 1);
    float Tinv = 1 / 298.15 + log(Rtherm / 10000.) / tempCalibB;


//    float R = 10000/ ((pow(2,15)/float(val))-1);
//    float To = 25.0f + 273.15f;
//    float Ro = 10000;
//    float B = 3428;
//    float Tinv = (1/To) + (log(R/Ro)/B);
    float T = (1/Tinv) - 273.15f;
    T = (T * 1.8f) +32;
    return T;
}


static float getTemperature(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 5.0f;

    return T;
}

static float getCurrent(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 4.096f;

    return T;
}

static float getVoltage(signed short val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<signed short>::max()) * 4.5f;

    return T;
}

static float getPhase(unsigned char val)
{
    float T;

    T = static_cast<float>(val) / static_cast<float>(std::numeric_limits<unsigned char>::max()) * 360.0f;

    return T;
}


static int sendTemperatureLimits(float val)
{
    int T;

    T = static_cast<int>(val / 5.0f * static_cast<float>(std::numeric_limits<signed short>::max()));

//    return T;
//    return (int)std::numeric_limits<signed short>::max();
    return 0;
}

static int sendCurrentLimit(float val)
{
    int T;

    T = static_cast<int>(val / 4.096f * static_cast<float>(std::numeric_limits<signed short>::max()));

    return T;
}

static int sendVoltageLimit(float val)
{
    int T;

    T = static_cast<int>(val / 4.5f * static_cast<float>(std::numeric_limits<signed short>::max()));

    return T;
}

static int sendReportingFrequency(float val)
{
    int T;

    T = static_cast<int>(val / 0.1f);

    return T;
}

static int sendCurrentSetpoint(float val)
{
    int T;

    if (val > 10.0f) {
        val = 10.0f;
    }

    T = static_cast<int>(val*64);

    return T;
}



static float getSineFrequency(int val)
{
    float T;

    T = static_cast<float>(val) * static_cast<float>(10000.0f) / static_cast<float>(256.0f);

    return T;
}

static int sendSineFrequency(float val)
{
    int T;

    T = static_cast<int>(val * 256.0f / 10000.0f);

    return T;
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

// Unit: Cells
#define MAXIMUM_NUM_CELLS                       10000   //?

// Unit: Volts
#define HIGH_VOLTAGE_CUTOFF_DEFAULT             0.0     //?
#define HIGH_VOLTAGE_CUTOFF_MIN                 0.0     //?
#define HIGH_VOLTAGE_CUTOFF_MAX                 0.0     //?

// Unit: Volts
#define LOW_VOLTAGE_CUTOFF_DEFAULT              0.0     //?
#define LOW_VOLTAGE_CUTOFF_MIN                  0.0     //?
#define LOW_VOLTAGE_CUTOFF_MAX                  0.0     //?

// Unit: Celsius
#define CHARGE_TEMP_CUTOFF_DEFAULT              45.0    //?
#define CHARGE_TEMP_CUTOFF_MIN                  0.0     //?
#define CHARGE_TEMP_CUTOFF_MAX                  0.0     //?

// Unit: Celsius
#define DISCHARGE_TEMP_CUTOFF_DEFAULT           0.0     //?
#define DISCHARGE_TEMP_CUTOFF_MIN               0.0     //?
#define DISCHARGE_TEMP_CUTOFF_MAX               0.0     //?

// Unit: Amps
#define CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT    4.096
#define CHARGE_CURRENT_SAFETY_CUTOFF_MIN        0.0     //?
#define CHARGE_CURRENT_SAFETY_CUTOFF_MAX        4.096

// Unit: Amps
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT 4.096
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN     0.0     //?
#define DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX     4.096

// Unit: Seconds
#define IMPEDANCE_REPORTING_PERIOD_DEFAULT      300
#define IMPEDANCE_REPORTING_PERIOD_MIN          0       //?
#define IMPEDANCE_REPORTING_PERIOD_MAX          0       //?

// Unit: Seconds
#define REPORTING_PERIOD_DEFAULT                0       //?
#define REPORTING_PERIOD_MIN                    0       //?
#define REPORTING_PERIOD_MAX                    0       //?

// Unit: Hz
#define SINE_WAVE_FREQUENCY_DEFAULT             0.0     //?
#define SINE_WAVE_FREQUENCY_MIN                 0.0     //?
#define SINE_WAVE_FREQUENCY_MAX                 0.0     //?

#define SINE_WAVE_MAGNITUDE_DEFAULT             0
#define SINE_WAVE_MAGNITUDE_MIN                 0
#define SINE_WAVE_MAGNITUDE_MAX                 2

// Unit: Cycles
#define NUM_WARMUP_CYCLES_DEFAULT               0
#define NUM_WARMUP_CYCLES_MIN                   0
#define NUM_WARMUP_CYCLES_MAX                   0       //?

// Unit: Cycles
#define NUM_MEASUREMENT_CYCLES_DEFAULT          1
#define NUM_MEASUREMENT_CYCLES_MIN              0
#define NUM_MEASUREMENT_CYCLES_MAX              0       //?

// Unit: Amps
#define CHARGE_RATE_DEFAULT                     0.0     //?
#define CHARGE_RATE_MIN                         0.0     //?
#define CHARGE_RATE_MAX                         0.0     //? I think we can remove this variable and use the set CHARGE_CURRENT_SAFETY_CUTOFF

// Unit: Amps
#define DISCHARGE_RATE_DEFAULT                  0.0     //?
#define DISCHARGE_RATE_MIN                      0.0     //?
#define DISCHARGE_RATE_MAX                      0.0     //? I think we can remove this variable and use the set DISCHARGE_CURRENT_SAFETY_CUTOFF

// Unit: Boolean
#define STORAGE_DISCHARGE_DEFAULT               true

// Unit: Volts
#define STORAGE_DISCHARGE_VOLTAGE_DEFAULT       0.0     //?
#define STORAGE_DISCHARGE_VOLTAGE_MIN           0.0     //? 0 makes sense to me but perhaps we want to make it something close but not exactly 0, in case the discharge gets asymptotic or something
#define STORAGE_DISCHARGE_VOLTAGE_MAX           0.0     //? I think we can remove this variable and use the maximum voltage of the cells, or maybe it needs to be something close but not exactly the max?

// Unit: Ohms
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT  0.0     //?
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN      0.0     //?
#define ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX      0.0     //?

// Unit: Seconds
#define REST_PERIOD_DEFAULT                     60
#define REST_PERIOD_MIN                         0
#define REST_PERIOD_MAX                         0       //?

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
    ERROR,
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

