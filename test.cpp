#include "test.h"
#include "ui_test.h"

batlabtest::batlabtest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);

    for (int i = 0; i < names.size(); ++i ) {
        ui->comboBox->addItem(names[vals(i)]);
    }

    for (int i = 0; i < writeNames.size(); ++i) {
        ui->regWriteBox->addItem(writeNames[writeVals(i)]);
    }

    connect(ui->regRead,SIGNAL(clicked()),this,SLOT(onRegRead()));
    connect(ui->regWrite,SIGNAL(clicked()),this,SLOT(onRegWrite()));
}

batlabtest::~batlabtest()
{
    delete ui;
}

void batlabtest::onRegRead() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),vals(ui->comboBox->currentIndex()));
}

void batlabtest::onRegWrite() {

    if (writeVals(ui->regWriteBox->currentIndex())!=writeVals::command) {
        qDebug() << ui->unitNumber_2->value() << ui->cellNumber_2->value() ;
        emit emitWriteReg(ui->unitNumber_2->value(),ui->cellNumber_2->value(),writeVals(ui->regWriteBox->currentIndex()),ui->spinBox->value());
    } else {
        qDebug() << ui->unitNumber_2->value() << 0 ;
        emit emitWriteReg(ui->unitNumber_2->value(),0,writeVals(ui->regWriteBox->currentIndex()),ui->spinBox->value());
    }
}
