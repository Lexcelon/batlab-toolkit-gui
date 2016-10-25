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
    connect(ui->regRead,SIGNAL(clicked()),this,SLOT(onRegRead()));
}

batlabtest::~batlabtest()
{
    delete ui;
}

void batlabtest::onRegRead() {
    emit emitReadReg(ui->unitNumber->value(),ui->cellNumber->value(),vals(ui->comboBox->currentIndex()));
}
