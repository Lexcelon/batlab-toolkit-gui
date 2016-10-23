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
    emit emitGetTemp(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetCurr() {
    emit emitGetCurr(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetCurrAmp() {
    emit emitGetCurrAmp(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetVol() {
    emit emitGetVol(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetVolAmp() {
    emit emitGetVolAmp(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetVolPhase() {
    emit emitGetVolPhase(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetEnergy() {
    emit emitGetEnergy(ui->unitNumber->value(),ui->cellNumber->value());
}

void batlabtest::onGetCharge() {
    emit emitGetCharge(ui->unitNumber->value(),ui->cellNumber->value());
}
