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

bool BatlabSettings::write(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open save file for new cell playlist.";
        return false;
    }

    QJsonObject playlistJson;

    playlistJson[CELL_PLAYLIST_NAME_FIELDSTR] = cellPlaylistName;

    playlistJson[BATLAB_CELL_PLAYLIST_FILE_VERSION_FIELDSTR] = batlabCellPlaylistFileVersion;

    playlistJson[NUM_WARMUP_CYCLES_FIELDSTR] = numWarmupCycles;
    playlistJson[NUM_MEASUREMENT_CYCLES_FIELDSTR] = numMeasurementCycles;

    playlistJson[STORAGE_DISCHARGE_FIELDSTR] = storageDischarge;
    playlistJson[STORAGE_DISCHARGE_VOLTAGE_FIELDSTR] = storageDischargeVoltage;

    playlistJson[REST_PERIOD_FIELDSTR] = restPeriod;

    playlistJson[HIGH_VOLTAGE_CUTOFF_FIELDSTR] = highVoltageCutoff;
    playlistJson[LOW_VOLTAGE_CUTOFF_FIELDSTR] = lowVoltageCutoff;

    playlistJson[CHARGE_TEMP_CUTOFF_FIELDSTR] = chargeTempCutoff;
    playlistJson[DISCHARGE_TEMP_CUTOFF_FIELDSTR] = dischargeTempCutoff;

    playlistJson[CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR] = chargeCurrentSafetyCutoff;
    playlistJson[DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR] = dischargeCurrentSafetyCutoff;

    playlistJson[PRECHARGE_RATE_FIELDSTR] = prechargeRate;
    playlistJson[CHARGE_RATE_FIELDSTR] = chargeRate;
    playlistJson[DISCHARGE_RATE_FIELDSTR] = dischargeRate;

    playlistJson[ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR] = acceptableImpedanceThreshold;

    playlistJson[REPORTING_PERIOD_FIELDSTR] = reportingPeriod;
    playlistJson[IMPEDANCE_REPORTING_PERIOD_FIELDSTR] = impedanceReportingPeriod;

    playlistJson[SINE_WAVE_FREQUENCY_FIELDSTR] = sineWaveFrequency;
    playlistJson[SINE_WAVE_MAGNITUDE_FIELDSTR] = sineWaveMagnitude;

    playlistJson[INDIVIDUAL_CELL_LOGS_FIELDSTR] = individualCellLogs;
    playlistJson[CELL_LOG_TIMESTAMPS_FIELDSTR] = cellLogTimestamps;

    playlistJson[PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR] = playlistOutputDirectory;
    playlistJson[PLAYLIST_SAVE_FILENAME_FIELDSTR] = playlistSaveFilename;

    QJsonArray cellNamesArray;
    for (int i = 0; i < cellNames.length(); i++) {
        cellNamesArray.append(cellNames[i]);
    }
    playlistJson[CELL_NAMES_FIELDSTR] = cellNamesArray;

    QJsonDocument saveDoc(playlistJson);
    file.write(saveDoc.toJson());
    file.close();

    return true;
}

bool BatlabSettings::load(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file to load cell playlist.";
        return false;
    }

    QString jsonString= file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();

    const QVector<QString> requiredFields = {
        CELL_PLAYLIST_NAME_FIELDSTR,
        BATLAB_CELL_PLAYLIST_FILE_VERSION_FIELDSTR,
        CELL_NAMES_FIELDSTR,
    };

    for(int i = 0; i < requiredFields.length(); i++) {
        if(!jsonObject.contains(requiredFields[i])) {
            qWarning() << "Cell playlist file did not contain required field: " << requiredFields[i];
            return false;
        }
    }

    setCellPlaylistName(jsonObject[CELL_PLAYLIST_NAME_FIELDSTR].toString());
    setBatlabCellPlaylistFileVersion(jsonObject[BATLAB_CELL_PLAYLIST_FILE_VERSION_FIELDSTR].toString());

    if(jsonObject.contains(NUM_WARMUP_CYCLES_FIELDSTR)) { setNumWarmupCycles(jsonObject[NUM_WARMUP_CYCLES_FIELDSTR].toInt()); }
    if(jsonObject.contains(NUM_MEASUREMENT_CYCLES_FIELDSTR)) { setNumMeasurementCycles(jsonObject[NUM_MEASUREMENT_CYCLES_FIELDSTR].toInt()); }
    if(jsonObject.contains(HIGH_VOLTAGE_CUTOFF_FIELDSTR)) { setHighVoltageCutoff(jsonObject[HIGH_VOLTAGE_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(LOW_VOLTAGE_CUTOFF_FIELDSTR)) { setLowVoltageCutoff(jsonObject[LOW_VOLTAGE_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(STORAGE_DISCHARGE_FIELDSTR)) { setStorageDischarge(jsonObject[STORAGE_DISCHARGE_FIELDSTR].toBool()); }
    if(jsonObject.contains(STORAGE_DISCHARGE_VOLTAGE_FIELDSTR)) { setStorageDischargeVoltage(jsonObject[STORAGE_DISCHARGE_VOLTAGE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(REST_PERIOD_FIELDSTR)) { setRestPeriod(jsonObject[REST_PERIOD_FIELDSTR].toInt()); }
    if(jsonObject.contains(CHARGE_TEMP_CUTOFF_FIELDSTR)) { setChargeTempCutoff(jsonObject[CHARGE_TEMP_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(DISCHARGE_TEMP_CUTOFF_FIELDSTR)) { setDischargeTempCutoff(jsonObject[DISCHARGE_TEMP_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR)) { setChargeCurrentSafetyCutoff(jsonObject[CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR)) { setDischargeCurrentSafetyCutoff(jsonObject[DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(PRECHARGE_RATE_FIELDSTR)) { setPrechargeRate(jsonObject[PRECHARGE_RATE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(CHARGE_RATE_FIELDSTR)) { setChargeRate(jsonObject[CHARGE_RATE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(DISCHARGE_RATE_FIELDSTR)) { setDischargeRate(jsonObject[DISCHARGE_RATE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR)) { setAcceptableImpedanceThreshold(jsonObject[ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR].toDouble()); }
    if(jsonObject.contains(REPORTING_PERIOD_FIELDSTR)) { setReportingPeriod(jsonObject[REPORTING_PERIOD_FIELDSTR].toDouble()); }
    if(jsonObject.contains(IMPEDANCE_REPORTING_PERIOD_FIELDSTR)) { setImpedanceReportingPeriod(jsonObject[IMPEDANCE_REPORTING_PERIOD_FIELDSTR].toDouble()); }
    if(jsonObject.contains(SINE_WAVE_FREQUENCY_FIELDSTR)) { setSineWaveFrequency(jsonObject[SINE_WAVE_FREQUENCY_FIELDSTR].toDouble()); }
    if(jsonObject.contains(SINE_WAVE_MAGNITUDE_FIELDSTR)) { setSineWaveMagnitude(jsonObject[SINE_WAVE_MAGNITUDE_FIELDSTR].toInt()); }
    if(jsonObject.contains(INDIVIDUAL_CELL_LOGS_FIELDSTR)) { setIndividualCellLogs(jsonObject[INDIVIDUAL_CELL_LOGS_FIELDSTR].toBool()); }
    if(jsonObject.contains(CELL_LOG_TIMESTAMPS_FIELDSTR)) { setCellLogTimestamps(jsonObject[CELL_LOG_TIMESTAMPS_FIELDSTR].toBool()); }
    if(jsonObject.contains(PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR)) { setPlaylistOutputDirectory(jsonObject[PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR].toString()); }
    if(jsonObject.contains(PLAYLIST_SAVE_FILENAME_FIELDSTR)) { setPlaylistSaveFilename(jsonObject[PLAYLIST_SAVE_FILENAME_FIELDSTR].toString()); }

    QJsonArray namesInFile = jsonObject[CELL_NAMES_FIELDSTR].toArray();
    QVector<QString> names;
    for(int i = 0; i < namesInFile.size(); i++) {
        names.append(namesInFile[i].toString());
    }
    setCellNames(names);

    return true;
}

bool BatlabSettings::setCellPlaylistName(QString str)
{
    cellPlaylistName = str;
    return true;
}

QString BatlabSettings::getCellPlaylistName() {
    return cellPlaylistName;
}

bool BatlabSettings::setBatlabCellPlaylistFileVersion(QString str)
{
    batlabCellPlaylistFileVersion = str;
    return true;
}

QString BatlabSettings::getBatlabCellPlaylistFileVersion() {
    return batlabCellPlaylistFileVersion;
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

int BatlabSettings::getNumWarmupCycles() {
    return numWarmupCycles;
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

int BatlabSettings::getNumMeasurementCycles() {
    return numMeasurementCycles;
}

bool BatlabSettings::setHighVoltageCutoff(double num)
{
    if(HIGH_VOLTAGE_CUTOFF_MIN <= num && num <= HIGH_VOLTAGE_CUTOFF_MAX)
    {
        highVoltageCutoff = num;
        return true;
    }
    else { return false; }
}

double BatlabSettings::getHighVoltageCutoff() {
    return highVoltageCutoff;
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

double BatlabSettings::getLowVoltageCutoff() {
    return lowVoltageCutoff;
}

bool BatlabSettings::setStorageDischarge(bool val)
{
    storageDischarge = val;
    return true;
}

bool BatlabSettings::getStorageDischarge() {
    return storageDischarge;
}

bool BatlabSettings::setStorageDischargeVoltage(double num)
{
    if(STORAGE_DISCHARGE_VOLTAGE_MIN <= num && num <= highVoltageCutoff) // Should set highVoltageCutoff first if it is not the default
    {
        storageDischargeVoltage = num;
        return true;
    }
    else { return false; }
}

double BatlabSettings::getStorageDischargeVoltage() {
    return storageDischargeVoltage;
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

int BatlabSettings::getRestPeriod() {
    return restPeriod;
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

double BatlabSettings::getChargeTempCutoff() {
    return chargeTempCutoff;
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

double BatlabSettings::getDischargeTempCutoff() {
    return dischargeTempCutoff;
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

double BatlabSettings::getChargeCurrentSafetyCutoff() {
    return chargeCurrentSafetyCutoff;
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

double BatlabSettings::getDischargeCurrentSafetyCutoff() {
    return dischargeCurrentSafetyCutoff;
}

bool BatlabSettings::setPrechargeRate(double num)
{
    if(PRECHARGE_RATE_MIN <= num && num <= chargeCurrentSafetyCutoff) // Should set chargeCurrentSafetyCutoff first if it is not the default
    {
        prechargeRate = num;
        return true;
    }
    else { return false; }
}

double BatlabSettings::getPrechargeRate() {
    return prechargeRate;
}

bool BatlabSettings::setChargeRate(double num)
{
    if(CHARGE_RATE_MIN <= num && num <= chargeCurrentSafetyCutoff) // Should set chargeCurrentSafetyCutoff first if it is not the default
    {
        chargeRate = num;
        return true;
    }
    else { return false; }
}

double BatlabSettings::getChargeRate() {
    return chargeRate;
}

bool BatlabSettings::setDischargeRate(double num)
{
    if(DISCHARGE_RATE_MIN <= num && num <= dischargeCurrentSafetyCutoff) // Should set dischargeCurrentSafetyCutoff first if it is not the default
    {
        dischargeRate = num;
        return true;
    }
    else { return false; }
}

double BatlabSettings::getDischargeRate() {
    return dischargeRate;
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

double BatlabSettings::getAcceptableImpedanceThreshold() {
    return acceptableImpedanceThreshold;
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

double BatlabSettings::getReportingPeriod() {
    return reportingPeriod;
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

double BatlabSettings::getImpedanceReportingPeriod() {
    return impedanceReportingPeriod;
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

double BatlabSettings::getSineWaveFrequency() {
    return sineWaveFrequency;
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

int BatlabSettings::getSineWaveMagnitude() {
    return sineWaveMagnitude;
}

bool BatlabSettings::setIndividualCellLogs(bool val)
{
    individualCellLogs = val;
    return true;
}

bool BatlabSettings::getIndividualCellLogs() {
    return individualCellLogs;
}

bool BatlabSettings::setCellLogTimestamps(bool val)
{
    cellLogTimestamps = val;
    return true;
}

bool BatlabSettings::getCellLogTimestamps() {
    return cellLogTimestamps;
}

bool BatlabSettings::setPlaylistOutputDirectory(QString str)
{
    playlistOutputDirectory = str;
    return true;
}

QString BatlabSettings::getPlaylistOutputDirectory() {
    return playlistOutputDirectory;
}

bool BatlabSettings::setPlaylistSaveFilename(QString str)
{
    playlistSaveFilename = str;
    return true;
}

QString BatlabSettings::getPlaylistSaveFilename() {
    return playlistSaveFilename;
}

bool BatlabSettings::setCellNames(QVector<QString> names)
{
    cellNames = names;
    return true;
}

QVector<QString> BatlabSettings::getCellNames() {
    return cellNames;
}