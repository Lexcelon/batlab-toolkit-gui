#include "batlabsettings.h"

BatlabSettings::BatlabSettings()
{
    cellPlaylistName = "";
    batlabCellPlaylistFileVersion = BATLAB_CELL_PLAYLIST_FILE_VERSION;
    numWarmupCycles = NUM_WARMUP_CYCLES_DEFAULT;
    numMeasurementCycles = NUM_MEASUREMENT_CYCLES_DEFAULT;
    highVoltageCutoff = HIGH_VOLTAGE_CUTOFF_DEFAULT;
    lowVoltageCutoff = LOW_VOLTAGE_CUTOFF_DEFAULT;
    storageDischarge = STORAGE_DISCHARGE_DEFAULT;
    storageDischargeVoltage = STORAGE_DISCHARGE_VOLTAGE_DEFAULT;
    restPeriod = REST_PERIOD_DEFAULT;
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
    cellNames = QVector<QString>(0);
}

bool BatlabSettings::write(QFile *file)
{
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open save file for new cell playlist.";
        return false;
    }

//    QJsonObject playlistJsonObject = this->jsonFromNewPlaylistWizard();

//    QJsonDocument saveDoc(playlistJsonObject);
//    file->write(saveDoc.toJson());
    return true;
}

bool BatlabSettings::load(QFile *file)
{
    return true;
}

QString BatlabSettings::toString() // TODO?
{
    return "";
}

bool BatlabSettings::setCellPlaylistName(QString str)
{
    cellPlaylistName = str;
    return true;
}

bool BatlabSettings::setBatlabCellPlaylistFileVersion(QString str)
{
    batlabCellPlaylistFileVersion = str;
    return true;
}

bool BatlabSettings::setNumWarmupCycles(int num)
{
    if(NUM_WARMUP_CYCLES_MIN <= num && num <= NUM_WARMUP_CYCLES_MAX)
    {
        numWarmupCycles = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setNumMeasurementCycles(int num)
{
    if(NUM_MEASUREMENT_CYCLES_MIN <= num && num <= NUM_MEASUREMENT_CYCLES_MAX)
    {
        numMeasurementCycles = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::sethighVoltageCutoff(double num)
{
    if(HIGH_VOLTAGE_CUTOFF_MIN <= num && num <= HIGH_VOLTAGE_CUTOFF_MAX)
    {
        highVoltageCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setLowVoltageCutoff(double num)
{
    if(LOW_VOLTAGE_CUTOFF_MIN <= num && num <= LOW_VOLTAGE_CUTOFF_MAX)
    {
        lowVoltageCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setStorageDischarge(bool val)
{
    storageDischarge = val;
    return true;
}

bool BatlabSettings::setStorageDischargeVoltage(double num)
{
    if(STORAGE_DISCHARGE_VOLTAGE_MIN <= num && num <= highVoltageCutoff) // Should set highVoltageCutoff first
    {
        storageDischargeVoltage = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setRestPeriod(int num)
{
    if(REST_PERIOD_MIN <= num && num <= REST_PERIOD_MAX)
    {
        restPeriod = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setChargeTempCutoff(double num)
{
    if(CHARGE_TEMP_CUTOFF_MIN <= num && num <= CHARGE_TEMP_CUTOFF_MAX)
    {
        chargeTempCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setDischargeTempCutoff(double num)
{
    if(DISCHARGE_TEMP_CUTOFF_MIN <= num && num <= DISCHARGE_TEMP_CUTOFF_MAX)
    {
        dischargeTempCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setChargeCurrentSafetyCutoff(double num)
{
    if(CHARGE_CURRENT_SAFETY_CUTOFF_MIN <= num && num <= CHARGE_CURRENT_SAFETY_CUTOFF_MAX)
    {
        chargeCurrentSafetyCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setDischargeCurrentSafetyCutoff(double num)
{
    if(DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN <= num && num <= DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX)
    {
        dischargeCurrentSafetyCutoff = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setPrechargeRate(double num)
{
    if(PRECHARGE_RATE_MIN <= num && num <= chargeCurrentSafetyCutoff) // Should set chargeCurrentSafetyCutoff first
    {
        prechargeRate = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setChargeRate(double num)
{
    if(CHARGE_RATE_MIN <= num) // TODO check against upper bound
    {
        chargeRate = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setDischargeRate(double num)
{
    if(DISCHARGE_RATE_MIN <= num) // TODO check against upper bound
    {
        dischargeRate = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setAcceptableImpedanceThreshold(double num)
{
    if(ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN <= num && num <= ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX)
    {
        acceptableImpedanceThreshold = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setReportingPeriod(double num)
{
    if(REPORTING_PERIOD_MIN <= num && num <= REPORTING_PERIOD_MAX)
    {
        reportingPeriod = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setImpedanceReportingPeriod(double num)
{
    if(IMPEDANCE_REPORTING_PERIOD_MIN <= num && num <= IMPEDANCE_REPORTING_PERIOD_MAX)
    {
        impedanceReportingPeriod = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setSineWaveFrequency(double num)
{
    if(SINE_WAVE_FREQUENCY_MIN <= num && num <= SINE_WAVE_FREQUENCY_MAX)
    {
        sineWaveFrequency = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setSineWaveMagnitude(int num)
{
    if(SINE_WAVE_MAGNITUDE_MIN <= num && num <= SINE_WAVE_MAGNITUDE_MAX)
    {
        sineWaveMagnitude = num;
        return true;
    }
    else { return false; }
}

bool BatlabSettings::setIndividualCellLogs(bool val)
{
    individualCellLogs = val;
    return true;
}

bool BatlabSettings::setCellLogTimestamps(bool val)
{
    cellLogTimestamps = val;
    return true;
}

bool BatlabSettings::setPlaylistOutputDirectory(QString str)
{
    playlistOutputDirectory = str;
    return true;
}

bool BatlabSettings::setPlaylistSaveFilename(QString str)
{
    playlistSaveFilename = str;
    return true;
}

bool BatlabSettings::setCellNames(QVector<QString> names)
{
    cellNames = names;
    return true;
}
