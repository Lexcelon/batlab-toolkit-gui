#include "wizardpagetwo.h"
#include "ui_wizardpagetwo.h"

wizardPageTwo::wizardPageTwo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageTwo)
{
    ui->setupUi(this);


    connect(this->ui->ccr,SIGNAL(valueChanged(double)),this,SLOT(onChargeCRate(double)));

    connect(this->ui->dcr,SIGNAL(valueChanged(double)),this,SLOT(onDischargeCRate(double)));
    connect(this->ui->ccSetpoint,SIGNAL(valueChanged(double)),this,SLOT(onConstantCurrentSetpoint(double)));
    connect(this->ui->ltCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowTempCutoff(double)));
    connect(this->ui->htCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighTempCutoff(double)));

    connect(this->ui->lvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowVoltageCutoff(double)));
    connect(this->ui->hvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighVoltageCutoff(double)));

    connect(this->ui->rf,SIGNAL(valueChanged(double)),this,SLOT(onReportingFrequency(double)));
    connect(this->ui->sf,SIGNAL(valueChanged(double)),this,SLOT(onSinewaveFrequency(double)));

    connect(this->ui->cycles,SIGNAL(valueChanged(int)),this,SLOT(onCycles(int)));

}

wizardPageTwo::~wizardPageTwo()
{
    delete ui;
}

void wizardPageTwo::onUpdate() {
    emit emitCycles(ui->cycles->value());
    emit emitParms(parms);
    emit emitDCR(ui->dcr->value());
    emit emitCCR(ui->ccr->value());
}
