#include "wizardpageone.h"
#include "ui_wizardpageone.h"

wizardPageOne::wizardPageOne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageOne)
{
    ui->setupUi(this);

    ui->startingIndex->setValidator(new QIntValidator(0,0xFFFF));

  //  connect(ui->startingIndex,SIGNAL(editingFinished()),this,SLOT(onFinish()));
}

void wizardPageOne::onFinish() {
    QString txt = ui->startingIndex->text();
    int zeros = txt.length() - QString::number(txt.toInt()).length()+1;
    txt = QString::number(txt.toInt()+1);
    ui->startingIndex->setText(QString("%1").arg(txt.toInt(),zeros,10,QChar('0')));
}

wizardPageOne::~wizardPageOne()
{
    delete ui;
}
