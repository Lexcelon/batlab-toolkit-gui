#include "batlabdebugdialog.h"
#include "ui_batlabdebugdialog.h"

BatlabDebugDialog::BatlabDebugDialog(QWidget *parent, QVector<Batlab *> com) :
    QDialog(parent),
    ui(new Ui::debugDialog)
{
    // TODO automatically switch to live view when opening?
    ui->setupUi(this);

    comObjects = com;
//    for (int i = 0; i < names.size(); ++i ) {
//        ui->comboBox->addItem(names[vals(i)]);
//    }

//    for (int i = 0; i < writeNames.size(); ++i) {
//        ui->regWriteBox->addItem(writeNames[writeVals(i)]);
//    }

//    for (int i = 0; i < cellProperties.size(); ++i) {
//        ui->cellComboBox->addItem(cellProperties[properties(i)]);
//    }
    for (int i = 0; i < comObjects.size(); ++i) {
        ui->comboBox->addItem(QString::number(com[i]->getSerialNumber()));
    }
    connect(ui->regRead,SIGNAL(clicked()),this,SLOT(onRegRead()));
    connect(ui->regWrite,SIGNAL(clicked()),this,SLOT(onRegWrite()));
}

BatlabDebugDialog::~BatlabDebugDialog()
{
    delete ui;
}

void BatlabDebugDialog::onRegRead() {
    comObjects[ui->comboBox->currentIndex()]->initiateRegisterRead(ui->unitNumber->value(),ui->cellNumber->value());
//    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value());
}

void BatlabDebugDialog::onRegWrite() {
    comObjects[ui->comboBox->currentIndex()]->initiateRegisterWrite(ui->unitNumber_2->value(),ui->cellNumber_2->value(),ui->spinBox->value());
//    emit emitWriteReg(ui->unitNumber_2->value(),ui->cellNumber_2->value(),ui->spinBox->value());
}

//void batlabtest::onPrintCell() {
//    emit emitPrint((uchar)((ui->unitNumber_3->value()<<2) + ui->cellNumber_3->value()), properties(ui->cellComboBox->currentIndex()));
//}
