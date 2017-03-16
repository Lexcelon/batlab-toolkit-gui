#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    this->setModal(true);

    batlabSettings = new batlabConfig();

    connect(this->ui->ccCutoff,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onChargeCurrentSafetyCutoff(double)));

    connect(this->ui->dcCutoff,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onDischargeCurrentSafetyCutoff(double)));
    connect(this->ui->ccSetpoint,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onConstantCurrentSetpoint(double)));
    connect(this->ui->ltCutoffSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onLowTempCutoff(double)));
    connect(this->ui->htCutoffSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onHighTempCutoff(double)));

    connect(this->ui->lvCutoffSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onLowVoltageCutoff(double)));
    connect(this->ui->hvCutoffSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onHighVoltageCutoff(double)));
    connect(this->ui->lvCVSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onLowVoltageCV(double)));
    connect(this->ui->hvCVSB,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onHighVoltageCV(double)));

    connect(this->ui->rf,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onReportingFrequency(double)));
    connect(this->ui->pOffT,SIGNAL(valueChanged(int)),batlabSettings,SLOT(onPulseOffTime(int)));
    connect(this->ui->pOnT,SIGNAL(valueChanged(int)),batlabSettings,SLOT(onPulseOnTime(int)));
    connect(this->ui->sf,SIGNAL(valueChanged(double)),batlabSettings,SLOT(onSinewaveFrequency(double)));

    connect(batlabSettings,SIGNAL(emitChargeCurrentSafetyCutoff(double)),this->ui->ccCutoff,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitDischargeCurrentSafetyCutoff(double)),this->ui->dcCutoff,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitConstantCurrentSetpoint(double)),this->ui->ccSetpoint,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitLowTempCutoff(double)),this->ui->ltCutoffSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitHighTempCutoff(double)),this->ui->htCutoffSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitLowVoltageCutoff(double)),this->ui->lvCutoffSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitHighVoltageCutoff(double)),this->ui->hvCutoffSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitLowVoltageCV(double)),this->ui->lvCVSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitHighVoltageCV(double)),this->ui->hvCVSB,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitReportingFrequency(double)),this->ui->rf,SLOT(setValue(double)));
    connect(batlabSettings,SIGNAL(emitSinewaveFrequency(double)),this->ui->sf,SLOT(setValue(double)));


    connect(this->ui->pbLoad,SIGNAL(clicked()),batlabSettings,SLOT(getDefaults()));
    connect(this->ui->pbSave,SIGNAL(clicked()),this,SLOT(saveSettings()));
    connect(this->ui->pbOK,SIGNAL(clicked()),this,SLOT(hide()));
//    batlabSettings->getDefaults();

}

void settings::saveSettings() {
    batlabConfig * config = batlabSettings;
//    emit emitSettings(name, config);
}

settings::~settings()
{
    delete batlabSettings;
    delete ui;
}
