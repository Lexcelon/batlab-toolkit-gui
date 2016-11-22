#ifndef WIZARDPAGETWO_H
#define WIZARDPAGETWO_H

#include <QWidget>
#include "globals.h"

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
    void onHighVoltageCutoff(double val) { parms.hvc = val;   emit emitParms(parms); }
    void onLowVoltageCutoff(double val) { parms.lvc = val;   emit emitParms(parms); }
    void onHighVoltageCV(double val) { parms.hvcv = val;   emit emitParms(parms); }
    void onLowVoltageCV(double val) { parms.lvcv = val;   emit emitParms(parms); }
    void onHighTempCutoff(double val) { parms.htc = val;   emit emitParms(parms); }
    void onLowTempCutoff(double val) { parms.ltc = val;   emit emitParms(parms); }
    void onChargeCRate(double val) { ccr = val;   emit emitCCR(ccr); }
    void onDischargeCRate(double val) { dcr = val;   emit emitDCR(dcr); }
    void onReportingFrequency(double val) { parms.rf = val;   emit emitParms(parms); }
    void onConstantCurrentSetpoint(double val) { parms.ccs = val;   emit emitParms(parms); }
    void onSinewaveFrequency(double val) { parms.sf = val;   emit emitParms(parms); }
    void onPulseOnTime(int seconds) { parms.poft = seconds;   emit emitParms(parms); }
    void onPulseOffTime(int seconds) { parms.pont = seconds;   emit emitParms(parms); }

    void onUpdate();

signals:
    void emitCycles(int);
    void emitParms(testParms);
    void emitCCR(double);
    void emitDCR(double);

private:
    Ui::wizardPageTwo *ui;
    testParms parms;
    int cycles;
    double ccr;
    double dcr;
};

#endif // WIZARDPAGETWO_H
