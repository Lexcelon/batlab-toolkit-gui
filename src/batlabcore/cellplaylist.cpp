#include "cellplaylist.h"

CellPlaylist::CellPlaylist()
{
    cellPlaylistName = "";
    batlabCellPlaylistFileVersion = BATLAB_CELL_PLAYLIST_FILE_VERSION;
    numWarmupCycles = NUM_WARMUP_CYCLES_DEFAULT;
    numMeasurementCycles = NUM_MEASUREMENT_CYCLES_DEFAULT;
    storageDischarge = STORAGE_DISCHARGE_DEFAULT;
    storageDischargeVoltage = STORAGE_DISCHARGE_VOLTAGE_DEFAULT;
    restPeriod = REST_PERIOD_DEFAULT;
    acceptableImpedanceThreshold = ACCEPTABLE_IMPEDANCE_THRESHOLD_DEFAULT;
    reportingPeriod = REPORTING_PERIOD_DEFAULT;
    impedanceReportingPeriod = IMPEDANCE_REPORTING_PERIOD_DEFAULT;
    individualCellLogs = INDIVIDUAL_CELL_LOGS_DEFAULT;
    cellLogTimestamps = CELL_LOG_TIMESTAMPS_DEFAULT;
    playlistOutputDirectory = "";
    playlistSaveFilename = "";
    cellNames = QVector<QString>(0);

    playlistBatlabSettings.highVoltageCutoff = HIGH_VOLTAGE_CUTOFF_DEFAULT;
    playlistBatlabSettings.lowVoltageCutoff = LOW_VOLTAGE_CUTOFF_DEFAULT;
    playlistBatlabSettings.chargeTempCutoff = CHARGE_TEMP_CUTOFF_DEFAULT;
    playlistBatlabSettings.dischargeTempCutoff = DISCHARGE_TEMP_CUTOFF_DEFAULT;
    playlistBatlabSettings.chargeCurrentSafetyCutoff = CHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT;
    playlistBatlabSettings.dischargeCurrentSafetyCutoff = DISCHARGE_CURRENT_SAFETY_CUTOFF_DEFAULT;
    playlistBatlabSettings.prechargeRate = PRECHARGE_RATE_DEFAULT;
    playlistBatlabSettings.chargeRate = CHARGE_RATE_DEFAULT;
    playlistBatlabSettings.dischargeRate = DISCHARGE_RATE_DEFAULT;
    playlistBatlabSettings.sineWaveFrequency = SINE_WAVE_FREQUENCY_DEFAULT;
    playlistBatlabSettings.sineWaveMagnitude = SINE_WAVE_MAGNITUDE_DEFAULT;
}



bool CellPlaylist::write(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open save file for new cell playlist.";
        return false;
    }

    QJsonObject playlistJson;
    QJsonObject batlabSettingsJson;

    playlistJson[CELL_PLAYLIST_NAME_FIELDSTR] = cellPlaylistName;

    playlistJson[BATLAB_CELL_PLAYLIST_FILE_VERSION_FIELDSTR] = batlabCellPlaylistFileVersion;

    playlistJson[NUM_WARMUP_CYCLES_FIELDSTR] = numWarmupCycles;
    playlistJson[NUM_MEASUREMENT_CYCLES_FIELDSTR] = numMeasurementCycles;

    playlistJson[STORAGE_DISCHARGE_FIELDSTR] = storageDischarge;
    playlistJson[STORAGE_DISCHARGE_VOLTAGE_FIELDSTR] = storageDischargeVoltage;

    playlistJson[REST_PERIOD_FIELDSTR] = restPeriod;

    batlabSettingsJson[HIGH_VOLTAGE_CUTOFF_FIELDSTR] = playlistBatlabSettings.highVoltageCutoff;
    batlabSettingsJson[LOW_VOLTAGE_CUTOFF_FIELDSTR] = playlistBatlabSettings.lowVoltageCutoff;

    batlabSettingsJson[CHARGE_TEMP_CUTOFF_FIELDSTR] = playlistBatlabSettings.chargeTempCutoff;
    batlabSettingsJson[DISCHARGE_TEMP_CUTOFF_FIELDSTR] = playlistBatlabSettings.dischargeTempCutoff;

    batlabSettingsJson[CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR] = playlistBatlabSettings.chargeCurrentSafetyCutoff;
    batlabSettingsJson[DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR] = playlistBatlabSettings.dischargeCurrentSafetyCutoff;

    batlabSettingsJson[PRECHARGE_RATE_FIELDSTR] = playlistBatlabSettings.prechargeRate;
    batlabSettingsJson[CHARGE_RATE_FIELDSTR] = playlistBatlabSettings.chargeRate;
    batlabSettingsJson[DISCHARGE_RATE_FIELDSTR] = playlistBatlabSettings.dischargeRate;

    playlistJson[ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR] = acceptableImpedanceThreshold;

    playlistJson[REPORTING_PERIOD_FIELDSTR] = reportingPeriod;
    playlistJson[IMPEDANCE_REPORTING_PERIOD_FIELDSTR] = impedanceReportingPeriod;

    batlabSettingsJson[SINE_WAVE_FREQUENCY_FIELDSTR] = playlistBatlabSettings.sineWaveFrequency;
    batlabSettingsJson[SINE_WAVE_MAGNITUDE_FIELDSTR] = playlistBatlabSettings.sineWaveMagnitude;

    playlistJson[INDIVIDUAL_CELL_LOGS_FIELDSTR] = individualCellLogs;
    playlistJson[CELL_LOG_TIMESTAMPS_FIELDSTR] = cellLogTimestamps;

    playlistJson[PLAYLIST_OUTPUT_DIRECTORY_FIELDSTR] = playlistOutputDirectory;
    playlistJson[PLAYLIST_SAVE_FILENAME_FIELDSTR] = playlistSaveFilename;

    playlistJson[BATLAB_SETTINGS_FIELDSTR] = batlabSettingsJson;

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

bool CellPlaylist::load(QString filename)
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
        BATLAB_SETTINGS_FIELDSTR
    };

    QJsonObject batlabSettingsJsonObject = jsonObject[BATLAB_SETTINGS_FIELDSTR].toObject();

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
    if(batlabSettingsJsonObject.contains(HIGH_VOLTAGE_CUTOFF_FIELDSTR)) { setHighVoltageCutoff(batlabSettingsJsonObject[HIGH_VOLTAGE_CUTOFF_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(LOW_VOLTAGE_CUTOFF_FIELDSTR)) { setLowVoltageCutoff(batlabSettingsJsonObject[LOW_VOLTAGE_CUTOFF_FIELDSTR].toDouble()); }
    if(jsonObject.contains(STORAGE_DISCHARGE_FIELDSTR)) { setStorageDischarge(jsonObject[STORAGE_DISCHARGE_FIELDSTR].toBool()); }
    if(jsonObject.contains(STORAGE_DISCHARGE_VOLTAGE_FIELDSTR)) { setStorageDischargeVoltage(jsonObject[STORAGE_DISCHARGE_VOLTAGE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(REST_PERIOD_FIELDSTR)) { setRestPeriod(jsonObject[REST_PERIOD_FIELDSTR].toInt()); }
    if(batlabSettingsJsonObject.contains(CHARGE_TEMP_CUTOFF_FIELDSTR)) { setChargeTempCutoff(batlabSettingsJsonObject[CHARGE_TEMP_CUTOFF_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(DISCHARGE_TEMP_CUTOFF_FIELDSTR)) { setDischargeTempCutoff(batlabSettingsJsonObject[DISCHARGE_TEMP_CUTOFF_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR)) { setChargeCurrentSafetyCutoff(batlabSettingsJsonObject[CHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR)) { setDischargeCurrentSafetyCutoff(batlabSettingsJsonObject[DISCHARGE_CURRENT_SAFETY_CUTOFF_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(PRECHARGE_RATE_FIELDSTR)) { setPrechargeRate(batlabSettingsJsonObject[PRECHARGE_RATE_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(CHARGE_RATE_FIELDSTR)) { setChargeRate(batlabSettingsJsonObject[CHARGE_RATE_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(DISCHARGE_RATE_FIELDSTR)) { setDischargeRate(batlabSettingsJsonObject[DISCHARGE_RATE_FIELDSTR].toDouble()); }
    if(jsonObject.contains(ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR)) { setAcceptableImpedanceThreshold(jsonObject[ACCEPTABLE_IMPEDANCE_THRESHOLD_FIELDSTR].toDouble()); }
    if(jsonObject.contains(REPORTING_PERIOD_FIELDSTR)) { setReportingPeriod(jsonObject[REPORTING_PERIOD_FIELDSTR].toDouble()); }
    if(jsonObject.contains(IMPEDANCE_REPORTING_PERIOD_FIELDSTR)) { setImpedanceReportingPeriod(jsonObject[IMPEDANCE_REPORTING_PERIOD_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(SINE_WAVE_FREQUENCY_FIELDSTR)) { setSineWaveFrequency(batlabSettingsJsonObject[SINE_WAVE_FREQUENCY_FIELDSTR].toDouble()); }
    if(batlabSettingsJsonObject.contains(SINE_WAVE_MAGNITUDE_FIELDSTR)) { setSineWaveMagnitude(batlabSettingsJsonObject[SINE_WAVE_MAGNITUDE_FIELDSTR].toInt()); }
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

bool CellPlaylist::setCellPlaylistName(QString str)
{
    cellPlaylistName = str;
    return true;
}

QString CellPlaylist::getCellPlaylistName() {
    return cellPlaylistName;
}

bool CellPlaylist::setBatlabCellPlaylistFileVersion(QString str)
{
    batlabCellPlaylistFileVersion = str;
    return true;
}

QString CellPlaylist::getBatlabCellPlaylistFileVersion() {
    return batlabCellPlaylistFileVersion;
}

bool CellPlaylist::setNumWarmupCycles(int num)
{
    if(NUM_WARMUP_CYCLES_MIN <= num && num <= NUM_WARMUP_CYCLES_MAX)
    {
        numWarmupCycles = num;
        return true;
    }
    else { return false; }
}

int CellPlaylist::getNumWarmupCycles() {
    return numWarmupCycles;
}

bool CellPlaylist::setNumMeasurementCycles(int num)
{
    if(NUM_MEASUREMENT_CYCLES_MIN <= num && num <= NUM_MEASUREMENT_CYCLES_MAX)
    {
        numMeasurementCycles = num;
        return true;
    }
    else { return false; }
}

int CellPlaylist::getNumMeasurementCycles() {
    return numMeasurementCycles;
}

bool CellPlaylist::setHighVoltageCutoff(double num)
{
    if(HIGH_VOLTAGE_CUTOFF_MIN <= num && num <= HIGH_VOLTAGE_CUTOFF_MAX)
    {
        playlistBatlabSettings.highVoltageCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getHighVoltageCutoff() {
    return playlistBatlabSettings.highVoltageCutoff;
}

bool CellPlaylist::setLowVoltageCutoff(double num)
{
    if(LOW_VOLTAGE_CUTOFF_MIN <= num && num <= LOW_VOLTAGE_CUTOFF_MAX)
    {
        playlistBatlabSettings.lowVoltageCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getLowVoltageCutoff() {
    return playlistBatlabSettings.lowVoltageCutoff;
}

bool CellPlaylist::setStorageDischarge(bool val)
{
    storageDischarge = val;
    return true;
}

bool CellPlaylist::getStorageDischarge() {
    return storageDischarge;
}

bool CellPlaylist::setStorageDischargeVoltage(double num)
{
    if(STORAGE_DISCHARGE_VOLTAGE_MIN <= num && num <= playlistBatlabSettings.highVoltageCutoff) // Should set highVoltageCutoff first if it is not the default
    {
        storageDischargeVoltage = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getStorageDischargeVoltage() {
    return storageDischargeVoltage;
}

bool CellPlaylist::setRestPeriod(int num)
{
    if(REST_PERIOD_MIN <= num && num <= REST_PERIOD_MAX)
    {
        restPeriod = num;
        return true;
    }
    else { return false; }
}

int CellPlaylist::getRestPeriod() {
    return restPeriod;
}

bool CellPlaylist::setChargeTempCutoff(double num)
{
    if(CHARGE_TEMP_CUTOFF_MIN <= num && num <= CHARGE_TEMP_CUTOFF_MAX)
    {
        playlistBatlabSettings.chargeTempCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getChargeTempCutoff() {
    return playlistBatlabSettings.chargeTempCutoff;
}

bool CellPlaylist::setDischargeTempCutoff(double num)
{
    if(DISCHARGE_TEMP_CUTOFF_MIN <= num && num <= DISCHARGE_TEMP_CUTOFF_MAX)
    {
        playlistBatlabSettings.dischargeTempCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getDischargeTempCutoff() {
    return playlistBatlabSettings.dischargeTempCutoff;
}

bool CellPlaylist::setChargeCurrentSafetyCutoff(double num)
{
    if(CHARGE_CURRENT_SAFETY_CUTOFF_MIN <= num && num <= CHARGE_CURRENT_SAFETY_CUTOFF_MAX)
    {
        playlistBatlabSettings.chargeCurrentSafetyCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getChargeCurrentSafetyCutoff() {
    return playlistBatlabSettings.chargeCurrentSafetyCutoff;
}

bool CellPlaylist::setDischargeCurrentSafetyCutoff(double num)
{
    if(DISCHARGE_CURRENT_SAFETY_CUTOFF_MIN <= num && num <= DISCHARGE_CURRENT_SAFETY_CUTOFF_MAX)
    {
        playlistBatlabSettings.dischargeCurrentSafetyCutoff = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getDischargeCurrentSafetyCutoff() {
    return playlistBatlabSettings.dischargeCurrentSafetyCutoff;
}

bool CellPlaylist::setPrechargeRate(double num)
{
    if(PRECHARGE_RATE_MIN <= num && num <= playlistBatlabSettings.chargeCurrentSafetyCutoff) // Should set chargeCurrentSafetyCutoff first if it is not the default
    {
        playlistBatlabSettings.prechargeRate = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getPrechargeRate() {
    return playlistBatlabSettings.prechargeRate;
}

bool CellPlaylist::setChargeRate(double num)
{
    if(CHARGE_RATE_MIN <= num && num <= playlistBatlabSettings.chargeCurrentSafetyCutoff) // Should set chargeCurrentSafetyCutoff first if it is not the default
    {
        playlistBatlabSettings.chargeRate = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getChargeRate() {
    return playlistBatlabSettings.chargeRate;
}

bool CellPlaylist::setDischargeRate(double num)
{
    if(DISCHARGE_RATE_MIN <= num && num <= playlistBatlabSettings.dischargeCurrentSafetyCutoff) // Should set dischargeCurrentSafetyCutoff first if it is not the default
    {
        playlistBatlabSettings.dischargeRate = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getDischargeRate() {
    return playlistBatlabSettings.dischargeRate;
}

bool CellPlaylist::setAcceptableImpedanceThreshold(double num)
{
    if(ACCEPTABLE_IMPEDANCE_THRESHOLD_MIN <= num && num <= ACCEPTABLE_IMPEDANCE_THRESHOLD_MAX)
    {
        acceptableImpedanceThreshold = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getAcceptableImpedanceThreshold() {
    return acceptableImpedanceThreshold;
}

bool CellPlaylist::setReportingPeriod(double num)
{
    if(REPORTING_PERIOD_MIN <= num && num <= REPORTING_PERIOD_MAX)
    {
        reportingPeriod = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getReportingPeriod() {
    return reportingPeriod;
}

bool CellPlaylist::setImpedanceReportingPeriod(double num)
{
    if(IMPEDANCE_REPORTING_PERIOD_MIN <= num && num <= IMPEDANCE_REPORTING_PERIOD_MAX)
    {
        impedanceReportingPeriod = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getImpedanceReportingPeriod() {
    return impedanceReportingPeriod;
}

bool CellPlaylist::setSineWaveFrequency(double num)
{
    if(SINE_WAVE_FREQUENCY_MIN <= num && num <= SINE_WAVE_FREQUENCY_MAX)
    {
        playlistBatlabSettings.sineWaveFrequency = num;
        return true;
    }
    else { return false; }
}

double CellPlaylist::getSineWaveFrequency() {
    return playlistBatlabSettings.sineWaveFrequency;
}

bool CellPlaylist::setSineWaveMagnitude(int num)
{
    if(SINE_WAVE_MAGNITUDE_MIN <= num && num <= SINE_WAVE_MAGNITUDE_MAX)
    {
        playlistBatlabSettings.sineWaveMagnitude = num;
        return true;
    }
    else { return false; }
}

int CellPlaylist::getSineWaveMagnitude() {
    return playlistBatlabSettings.sineWaveMagnitude;
}

bool CellPlaylist::setIndividualCellLogs(bool val)
{
    individualCellLogs = val;
    return true;
}

bool CellPlaylist::getIndividualCellLogs() {
    return individualCellLogs;
}

bool CellPlaylist::setCellLogTimestamps(bool val)
{
    cellLogTimestamps = val;
    return true;
}

bool CellPlaylist::getCellLogTimestamps() {
    return cellLogTimestamps;
}

bool CellPlaylist::setPlaylistOutputDirectory(QString str)
{
    playlistOutputDirectory = str;
    return true;
}

QString CellPlaylist::getPlaylistOutputDirectory() {
    return playlistOutputDirectory;
}

bool CellPlaylist::setPlaylistSaveFilename(QString str)
{
    playlistSaveFilename = str;
    return true;
}

QString CellPlaylist::getPlaylistSaveFilename() {
    return playlistSaveFilename;
}

bool CellPlaylist::setCellNames(QVector<QString> names)
{
    cellNames = names;
    return true;
}

QVector<QString> CellPlaylist::getCellNames() {
    return cellNames;
}
