#ifndef WIZARDPAGETWO_H
#define WIZARDPAGETWO_H

#include <QWidget>
#include "../globals.h"

namespace Ui {
class wizardPageTwo;
}

class wizardPageTwo : public QWidget
{
    Q_OBJECT

public:
    explicit wizardPageTwo(QWidget *parent = 0);
    ~wizardPageTwo();

public slots:
    void onCycles(int val) {cycles = val; emit emitCycles(cycles);}
    void onHighVoltageCutoff(double val) { parms.hightVoltageCutoff = val;   emit emitParms(parms); }
    void onLowVoltageCutoff(double val) { parms.lowVoltageCutoff = val;   emit emitParms(parms); }
    void onHighTempCutoff(double val) { parms.temperatureCutoffCharge = val;   emit emitParms(parms); }
    void onLowTempCutoff(double val) { parms.temperatureCutoffDischarge = val;   emit emitParms(parms); }
    void onCurrentChargeCutoff(double val) { parms.currentCutoffCharge = val;   emit emitParms(parms);}
    void onCurrentDischargeCutoff(double val) { parms.currentCutoffCharge = val;   emit emitParms(parms);}
    void onReportingFrequency(double val) { parms.reportingFrequency = val;   emit emitParms(parms); }
    void onChargeCurrentSetpoint(double val) { parms.chargeCurrentSetpoint = val;   emit emitParms(parms); }
    void onDischargeCurrentSetpoint(double val) { parms.dischargeCurrentSetpoint = val;   emit emitParms(parms); }
    void onCapacity(double val) { capacity = val;   emit emitCapacity(capacity); }
    void onRestTime(int val) { restTime = val;   emit emitRestTime(restTime); }

    void onUpdate();

signals:
    void emitCycles(int);
    void emitParms(testParms);
    void emitCapacity(double);
    void emitRestTime(int);

private:
    Ui::wizardPageTwo *ui;
    testParms parms;
    int cycles;
    double capacity;
    int restTime;
};

#endif // WIZARDPAGETWO_H
