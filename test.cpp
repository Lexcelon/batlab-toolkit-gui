#include "test.h"
#include "ui_test.h"

batlabtest::batlabtest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    connect(ui->tempButton,SIGNAL(clicked()),this,SLOT(onGetTemp()));
    connect(ui->currentButton,SIGNAL(clicked()),this,SLOT(onGetCurr()));
    connect(ui->currentAmpButton,SIGNAL(clicked()),this,SLOT(onGetCurrAmp()));
    connect(ui->voltageButton,SIGNAL(clicked()),this,SLOT(onGetVol()));
    connect(ui->voltagePhaseButton,SIGNAL(clicked()),this,SLOT(onGetVolPhase()));
    connect(ui->voltageAmpButton,SIGNAL(clicked()),this,SLOT(onGetVolAmp()));
    connect(ui->energyButton,SIGNAL(clicked()),this,SLOT(onGetEnergy()));
    connect(ui->chargeButton,SIGNAL(clicked()),this,SLOT(onGetCharge()));
}

batlabtest::~batlabtest()
{
    delete ui;
}

void batlabtest::onGetTemp() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),temperature);
}

void batlabtest::onGetCurr() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),current);
}

void batlabtest::onGetCurrAmp() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),currentAmplitude);
}

void batlabtest::onGetVol() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),voltage);
}

void batlabtest::onGetVolAmp() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),voltageAmplitude);
}

void batlabtest::onGetVolPhase() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),voltagePhase);
}

void batlabtest::onGetEnergy() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),energy);
}

void batlabtest::onGetCharge() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),charge);
}
