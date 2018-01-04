#include "batlabsettings.h"

BatlabSettings::BatlabSettings()
{
    cellPlaylistName = "";
    batlabCellPlaylistFileVersion = BATLAB_CELL_PLAYLIST_FILE_VERSION;
    numWarmupCycles = NUM_WARMUP_CYCLES_DEFAULT;
    numMeasurementCycles = NUM_MEASUREMENT_CYCLES_DEFAULT;
    storageDischarge = STORAGE_DISCHARGE_DEFAULT;
    storageDischargeVoltage = STORAGE_DISCHARGE_VOLTAGE_DEFAULT;
    restPeriod = REST_PERIOD_DEFAULT;
    highVoltageCutoff = HIGH_VOLTAGE_CUTOFF_DEFAULT;
    lowVoltageCutoff = LOW_VOLTAGE_CUTOFF_DEFAULT;
    chargeTempCutoff = CHARGE_TEMP_CUTOFF_DEFAULT;
    dischargeTempCutoff = DISCHARGE_TEMP_CUTOFF_DEFAULT;
    chargeCurrentSafetyCutoff = CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT;
    dischargeCurrentSafetyCutoff = DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT;
    prechargeRate = PRECHARGE_RATE_DEFAULT;
    chargeRate = CHARGE_RATE_DEFAULT;
    dischargeRate = DISCHARGE_RATE_DEFAULT;
    acceptableImpedanceThreshold = ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT;
    reportingPeriod = REPORTING_PERIOD_DEFAULT;
    impedanceReportingPeriod = IMPEDANCE_REPORTING_PERIOD_DEFAULT;
    sineWaveFrequency = SINE_WAVE_FREQUENCY_DEFAULT;
    sineWaveMagnitude = SINE_WAVE_MAGNITUDE_DEFAULT;
    individualCellLogs = INDIVIDUAL_CELL_LOGS_DEFAULT;
    cellLogTimestamps = CELL_LOG_TIMESTAMPS_DEFAULT;
    playlistOutputDirectory = "";
    playlistSaveFilename = "";
}
