#include "batlabconfig.h"

batlabConfig::batlabConfig()
{
    settings = new QSettings(QString("BaTlAbSeTtInGs.INI"),QSettings::IniFormat);
    qDebug() << settings->fileName();
    QStringList temp = settings->allKeys();

    if (temp.isEmpty()) {
        createSettings();
    } else {
        loadSettings();
    }
}

batlabConfig::batlabConfig(QString name) {
    settings = new QSettings(name + ".INI",QSettings::IniFormat);
    QStringList temp = settings->allKeys();
    if (temp.isEmpty()) {
        createSettings();
    } else {
        loadSettings();
    }
}

batlabConfig::~batlabConfig()
{

}

void batlabConfig::loadSettings() {
   hvCutoff = settings->value(QString("hvCutoff"),0.0f).toDouble();
   lvCutoff = settings->value(QString("lvCutoff"),0.0f).toDouble();
   hvCV = settings->value(QString("hvCV"),0.0f).toDouble();
   lvCV = settings->value(QString("lvCV"),0.0f).toDouble();
   htCutoff = settings->value(QString("htCutoff"),0.0f).toDouble();
   ltCutoff = settings->value(QString("ltCutoff"),0.0f).toDouble();
   ccCutoff = settings->value(QString("ccCutoff"),0.0f).toDouble();
   dcCutoff = settings->value(QString("dcCutoff"),0.0f).toDouble();
   rFreq = settings->value(QString("rFreq"),0.0f).toDouble();
   ccSetpoint = settings->value(QString("ccSetpoint"),0.0f).toDouble();
   sinFreq = settings->value(QString("sinFreq"),0.0f).toDouble();
   pulseOn = settings->value(QString("pulseOn"),0).toInt();
   pulseOff = settings->value(QString("pulseOff"),0).toInt();
}

void batlabConfig::createSettings() {
    hvCutoff = 4.2f;
    lvCutoff = 2.65f;
    hvCV = 4.2f;
    lvCV = 2.8f;
    htCutoff = 45.0f;
    ltCutoff = -INFINITY;
    ccCutoff = 3.0f;
    dcCutoff = 3.0f;
    rFreq = 1.0f;
    ccSetpoint = 2.0f;
    sinFreq = 1000.0f;
    pulseOn = 600;
    pulseOff = 600;
    update();
}

void batlabConfig::update() {
    settings->setValue(QString("hvCutoff"),hvCutoff);
    settings->setValue(QString("lvCutoff"),lvCutoff);
    settings->setValue(QString("hvCV"),hvCV);
    settings->setValue(QString("lvCV"),lvCV);
    settings->setValue(QString("htCutoff"),htCutoff);
    settings->setValue(QString("ltCutoff"),ltCutoff);
    settings->setValue(QString("ccCutoff"),ccCutoff);
    settings->setValue(QString("hvCutoff"),hvCutoff);
    settings->setValue(QString("dcCutoff"),dcCutoff);
    settings->setValue(QString("rFreq"),rFreq);
    settings->setValue(QString("ccSetpoint"),ccSetpoint);
    settings->setValue(QString("sinFreq"),sinFreq);
    settings->setValue(QString("pulseOn"),pulseOn);
    settings->setValue(QString("pulseOff"),pulseOff);
}

void batlabConfig::getDefaults() {
    emit emitHighVoltageCutoff(hvCutoff);
    emit emitLowVoltageCutoff(lvCutoff);
    emit emitHighVoltageCV(hvCV);
    emit emitLowVoltageCV(lvCV);
    emit emitHighTempCutoff(htCutoff);
    emit emitLowTempCutoff(ltCutoff);
    emit emitChargeCurrentSafetyCutoff(ccCutoff);
    emit emitDischargeCurrentSafetyCutoff(dcCutoff);
    emit emitReportingFrequency(rFreq);
    emit emitConstantCurrentSetpoint(ccSetpoint);
    emit emitSinewaveFrequency(sinFreq);
    emit emitPulseOnTime(pulseOn);
    emit emitPulseOffTime(pulseOff);
}

