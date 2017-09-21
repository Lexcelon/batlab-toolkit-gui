#include "wizardpagetwo.h"
#include "ui_wizardpagetwo.h"

wizardPageTwo::wizardPageTwo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageTwo)
{
    ui->setupUi(this);


    connect(this->ui->ccSetpoint,SIGNAL(valueChanged(double)),this,SLOT(onChargeCurrentSetpoint(double)));
    connect(this->ui->doubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(onDischargeCurrentSetpoint(double)));

    connect(this->ui->ltCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowTempCutoff(double)));
    connect(this->ui->htCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighTempCutoff(double)));

//    connect(this->ui->ccr,SIGNAL(valueChanged(double)),this,SLOT(onCurrentDischargeCutoff(double)));
//    connect(this->ui->dcr,SIGNAL(valueChanged(double)),this,SLOT(onCurrentChargeCutoff(double)));


    connect(this->ui->lvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onLowVoltageCutoff(double)));
    connect(this->ui->hvCutoffSB,SIGNAL(valueChanged(double)),this,SLOT(onHighVoltageCutoff(double)));

//    connect(this->ui->rf,SIGNAL(valueChanged(double)),this,SLOT(onReportingFrequency(double)));

    connect(this->ui->cycles,SIGNAL(valueChanged(int)),this,SLOT(onCycles(int)));
//    connect(this->ui->sbRestTime,SIGNAL(valueChanged(int)),this,SLOT(onRestTime(int)));
    connect(this->ui->capacity,SIGNAL(valueChanged(double)),this,SLOT(onCapacity(double)));

}

wizardPageTwo::~wizardPageTwo()
{
    delete ui;
}

void wizardPageTwo::onUpdate()
{
    emit emitCycles(ui->cycles->value());
    emit emitParms(parms);
    emit emitCapacity(ui->capacity->value());
//    emit emitRestTime(ui->sbRestTime->value());
}
