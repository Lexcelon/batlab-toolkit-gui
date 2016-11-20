#include "wizardpagethree.h"
#include "ui_wizardpagethree.h"

wizardPageThree::wizardPageThree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageThree)
{
    ui->setupUi(this);
}

wizardPageThree::~wizardPageThree()
{
    delete ui;
}


QString wizardPageThree::onGetName(int val) {
    int zeros = startInd.length() - QString::number(startInd.toInt()).length()+1;
    QString txt = QString::number(startInd.toInt()+val);
    return (QString("%1").arg(txt.toInt(),zeros,10,QChar('0')));
}

void wizardPageThree::onActivate() {
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < numCells; ++i) {
        ui->tableWidget->insertRow(i);



        QString tempName = designator + onGetName(i);
        QLabel * name = new QLabel(tempName);
        ui->tableWidget->setCellWidget(i,0,name);
        //QLineEdit * one = new QLineEdit();
    }
}


void wizardPageThree::onDesignator(QString val) {
    qDebug() << val;
    designator = val;
}

void wizardPageThree::onStartInd(QString val) {
    startInd = val;
}

void wizardPageThree::onNumCells(int val) {
    numCells = val;
}

void wizardPageThree::onTestParms(testParms val) {
    parms = val;
}

void wizardPageThree::onNumCycles(int val) {
    numCycles = val;
}
