#include "wizardpagetwo.h"
#include "ui_wizardpagetwo.h"

wizardPageTwo::wizardPageTwo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageTwo)
{
    ui->setupUi(this);


    connect(this->ui->ccCutoff,SIGNAL(valueChanged(double)),this,SLOT(onChargeCurrentSafetyCutoff(double)));

    connect(this->ui->dcCutoff,SIGNAL(valueChanged(double)),this,SLOT(onDischargeCurrentSafetyCutoff(double)));
    connect(this->ui->ccSetpoint,SIGNAL(valueChanged(double)),this,SLOT(onConstantCurrentSetpoint(double)));
    connect(this->ui->ltCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowTempCutoff(double)));
    connect(this->ui->htCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighTempCutoff(double)));

    connect(this->ui->lvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowVoltageCutoff(double)));
    connect(this->ui->hvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighVoltageCutoff(double)));
    connect(this->ui->lvCVSB,SIGNAL(valueChanged(double)),this,SLOT(onLowVoltageCV(double)));
    connect(this->ui->hvCVSB,SIGNAL(valueChanged(double)),this,SLOT(onHighVoltageCV(double)));

    connect(this->ui->rf,SIGNAL(valueChanged(double)),this,SLOT(onReportingFrequency(double)));
    connect(this->ui->pOffT,SIGNAL(valueChanged(int)),this,SLOT(onPulseOffTime(int)));
    connect(this->ui->pOnT,SIGNAL(valueChanged(int)),this,SLOT(onPulseOnTime(int)));
    connect(this->ui->sf,SIGNAL(valueChanged(double)),this,SLOT(onSinewaveFrequency(double)));

    connect(this->ui->cycles,SIGNAL(valueChanged(int)),this,SLOT(onCycles(int)));
}

wizardPageTwo::~wizardPageTwo()
{
    delete ui;
}
