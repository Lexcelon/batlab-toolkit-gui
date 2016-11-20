#include "wizardpagetwo.h"
#include "ui_wizardpagetwo.h"

wizardPageTwo::wizardPageTwo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageTwo)
{
    ui->setupUi(this);
}

wizardPageTwo::~wizardPageTwo()
{
    delete ui;
}
