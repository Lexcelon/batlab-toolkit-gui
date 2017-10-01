#ifndef BATLABCONFIG_H
#define BATLABCONFIG_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include "globals.h"


class batlabConfig : public QObject
{
    Q_OBJECT

public:
    batlabConfig();
    batlabConfig(QString name);
    ~batlabConfig();
    void createSettings();
    void loadSettings();

public slots:
    void onHighVoltageCutoff(double val) { hvCutoff = val; }
    void onLowVoltageCutoff(double val) { lvCutoff = val; }
    void onHighVoltageCV(double val) { hvCV = val; }
    void onLowVoltageCV(double val) { lvCV = val; }
    void onHighTempCutoff(double val) { htCutoff = val; }
    void onLowTempCutoff(double val) { ltCutoff = val; }
    void onChargeCurrentSafetyCutoff(double val) { ccCutoff = val; }
    void onDischargeCurrentSafetyCutoff(double val) { dcCutoff = val; }
    void onReportingFrequency(double val) { rFreq = val; }
    void onConstantCurrentSetpoint(double val) { ccSetpoint = val; }
    void onSinewaveFrequency(double val) { sinFreq = val; }
    void onPulseOnTime(int seconds) { pulseOn = seconds; }
    void onPulseOffTime(int seconds) { pulseOff = seconds; }
    void update();

    void getDefaults();


signals:
    void emitHighVoltageCutoff(double);
    void emitLowVoltageCutoff(double);
    void emitHighVoltageCV(double);
    void emitLowVoltageCV(double);
    void emitHighTempCutoff(double);
    void emitLowTempCutoff(double);
    void emitChargeCurrentSafetyCutoff(double);
    void emitDischargeCurrentSafetyCutoff(double);
    void emitReportingFrequency(double);
    void emitConstantCurrentSetpoint(double);
    void emitSinewaveFrequency(double);
    void emitPulseOnTime(int);
    void emitPulseOffTime(int);

private:
    QSettings * settings;
    double hvCutoff;
    double lvCutoff;
    double hvCV;
    double lvCV;
    double htCutoff;
    double ltCutoff;
    double ccCutoff;
    double dcCutoff;
    double rFreq;
    double ccSetpoint;
    double sinFreq;
    int pulseOn;
    int pulseOff;

};

#endif // BATLABCONFIG_H
