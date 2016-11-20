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

        QSpinBox * cycleNum = new QSpinBox();
        cycleNum->setMaximum(0xFFFF);
        cycleNum->setValue(numCycles);
        ui->tableWidget->setCellWidget(i,1,cycleNum);

        QDoubleSpinBox *  hvc = new QDoubleSpinBox();
        hvc->setValue(parms.hvc);
        ui->tableWidget->setCellWidget(i,2,hvc);

        QDoubleSpinBox *  lvc = new QDoubleSpinBox();
        lvc->setValue(parms.lvc);
        ui->tableWidget->setCellWidget(i,3,lvc);

        QDoubleSpinBox *  hvcv = new QDoubleSpinBox();
        hvcv->setValue(parms.hvcv);
        ui->tableWidget->setCellWidget(i,4,hvcv);

        QDoubleSpinBox *  lvcv = new QDoubleSpinBox();
        lvcv->setValue(parms.lvcv);
        ui->tableWidget->setCellWidget(i,5,lvcv);

        QDoubleSpinBox *  htc = new QDoubleSpinBox();
        htc->setValue(parms.htc);
        ui->tableWidget->setCellWidget(i,6,htc);

        QDoubleSpinBox *  ltc = new QDoubleSpinBox();
        ltc->setValue(parms.ltc);
        ui->tableWidget->setCellWidget(i,7,ltc);

        QDoubleSpinBox *  ccsc = new QDoubleSpinBox();
        ccsc->setValue(parms.ccsc);
        ui->tableWidget->setCellWidget(i,8,ccsc);

        QDoubleSpinBox *  dcsc = new QDoubleSpinBox();
        dcsc->setValue(parms.dcsc);
        ui->tableWidget->setCellWidget(i,9,dcsc);

        QDoubleSpinBox *  rf = new QDoubleSpinBox();
        rf->setValue(parms.rf);
        ui->tableWidget->setCellWidget(i,10,rf);

        QDoubleSpinBox *  ccs = new QDoubleSpinBox();
        ccs->setValue(parms.ccs);
        ui->tableWidget->setCellWidget(i,11,ccs);

        QDoubleSpinBox *  sf = new QDoubleSpinBox();
        sf->setValue(parms.sf);
        ui->tableWidget->setCellWidget(i,12,sf);

        QSpinBox * pont = new QSpinBox();
        pont->setMaximum(0xFFFF);
        pont->setValue(parms.pont);
        ui->tableWidget->setCellWidget(i,13,pont);

        QSpinBox * poft = new QSpinBox();
        poft->setMaximum(0xFFFF);
        poft->setValue(parms.poft);
        ui->tableWidget->setCellWidget(i,14,poft);

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
