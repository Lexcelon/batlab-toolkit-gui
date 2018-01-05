#ifndef BATLABSETTINGS_H
#define BATLABSETTINGS_H

#include <QString>
#include <QVector>
#include <QFile>
#include "globals.h"

class BatlabSettings
{
public:
    BatlabSettings();

    bool write(QFile *file);
    bool load(QFile *file);
    QString toString();
    
    bool setCellPlaylistName(QString str);
    bool setBatlabCellPlaylistFileVersion(QString str);
    bool setNumWarmupCycles(int num);
    bool setNumMeasurementCycles(int num);
    bool sethighVoltageCutoff(double num);
    bool setLowVoltageCutoff(double num);
    bool setStorageDischarge(bool val);
    bool setStorageDischargeVoltage(double num);
    bool setRestPeriod(int num);
    bool setChargeTempCutoff(double num);
    bool setDischargeTempCutoff(double num);
    bool setChargeCurrentSafetyCutoff(double num);
    bool setDischargeCurrentSafetyCutoff(double num);
    bool setPrechargeRate(double num);
    bool setChargeRate(double num);
    bool setDischargeRate(double num);
    bool setAcceptableImpedanceThreshold(double num);
    bool setReportingPeriod(double num);
    bool setImpedanceReportingPeriod(double num);
    bool setSineWaveFrequency(double num);
    bool setSineWaveMagnitude(int num);
    bool setIndividualCellLogs(bool val);
    bool setCellLogTimestamps(bool val);
    bool setPlaylistOutputDirectory(QString str);
    bool setPlaylistSaveFilename(QString str);
    bool setCellNames(QVector<QString> names);
    
private:
    QString cellPlaylistName;
    QString batlabCellPlaylistFileVersion;
    int numWarmupCycles;
    int numMeasurementCycles;
    double highVoltageCutoff;
    double lowVoltageCutoff;
    bool storageDischarge;
    double storageDischargeVoltage;
    int restPeriod;
    double chargeTempCutoff;
    double dischargeTempCutoff;
    double chargeCurrentSafetyCutoff;
    double dischargeCurrentSafetyCutoff;
    double prechargeRate;
    double chargeRate;
    double dischargeRate;
    double acceptableImpedanceThreshold;
    double reportingPeriod;
    double impedanceReportingPeriod;
    double sineWaveFrequency;
    int sineWaveMagnitude;
    bool individualCellLogs;
    bool cellLogTimestamps;
    QString playlistOutputDirectory;
    QString playlistSaveFilename;
    QVector<QString> cellNames;
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
#define PRECHARGE_RATE_FIELDSTR                             "prechargeRate"
#define PRECHARGE_RATE_DEFAULT                              2.0
#define PRECHARGE_RATE_MIN                                  0.25
// #define PRECHARGE_RATE_MAX                               CHARGE_CURRENT_SAFETY_CUTOFF used

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

// Unit: Boolean
//#define ENABLE_TRICKLE_FIELDSTR                             "enableTrickle"
//#define ENABLE_TRICKLE_DEFAULT                              false

// Unit: Boolean
//#define ENABLE_PULSE_FIELDSTR                               "enablePulse"
//#define ENABLE_PULSE_DEFAULT                                false

// Unit: Boolean
#define INDIVIDUAL_CELL_LOGS_FIELDSTR                       "individualCellLogs"
#define INDIVIDUAL_CELL_LOGS_DEFAULT                        true

// Unit: Boolean
#define CELL_LOG_TIMESTAMPS_FIELDSTR                        "cellLogTimestamps"
#define CELL_LOG_TIMESTAMPS_DEFAULT                         true

#define PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR                  "playlistOutputDirectory"
#define PLAYLIST_SAVE_FILENAME_FIELDSTR                     "playlistSaveFilename"

#endif // BATLABSETTINGS_H
