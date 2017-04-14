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
    int length = startInd.length();
   // int zeros = startInd.length() - QString::number(startInd.toInt()).length()+1;
    QString txt = QString::number(startInd.toInt()+val);
    if (txt.length() > length) {
        length = txt.length();
    }
    return (QString("%1").arg(txt.toInt(),length,10,QChar('0')));
}

void wizardPageThree::onActivate() {
    QProgressDialog bar(tr("Creating %1 cells from parameters...").arg(numCells), tr("Cancel"), 0, numCells-1, this);
    bar.setMinimumDuration(0);
//    bar.show();
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < numCells; ++i) {
        int index = 0;
        ui->tableWidget->insertRow(i);

        QString tempName = designator + onGetName(i);
        QLabel * name = new QLabel(tempName);
        ui->tableWidget->setCellWidget(i,index++,name);

        QSpinBox * cycleNum = new QSpinBox();
        cycleNum->setMaximum(0xFFFF);
        cycleNum->setValue(numCycles);
        ui->tableWidget->setCellWidget(i,index++,cycleNum);

        QSpinBox * sbRestTime = new QSpinBox();
        sbRestTime->setMaximum(0xFFFF);
        sbRestTime->setValue(restTime);
        ui->tableWidget->setCellWidget(i,index++,sbRestTime);

        QDoubleSpinBox *  sbHighVoltageCutoff = new QDoubleSpinBox();
        sbHighVoltageCutoff->setValue(parms.hightVoltageCutoff);
        ui->tableWidget->setCellWidget(i,index++,sbHighVoltageCutoff);

        QDoubleSpinBox *  sbLowVoltageCutoff= new QDoubleSpinBox();
        sbLowVoltageCutoff->setValue(parms.lowVoltageCutoff);
        ui->tableWidget->setCellWidget(i,index++,sbLowVoltageCutoff);

        QDoubleSpinBox *  sbTemperatureCutoffCharge = new QDoubleSpinBox();
        sbTemperatureCutoffCharge->setValue(parms.temperatureCutoffCharge);
        ui->tableWidget->setCellWidget(i,index++,sbTemperatureCutoffCharge);

        QDoubleSpinBox *  sbTemperatureCutoffDischarge = new QDoubleSpinBox();
        sbTemperatureCutoffDischarge->setValue(parms.temperatureCutoffDischarge);
        ui->tableWidget->setCellWidget(i,index++,sbTemperatureCutoffDischarge);

        QDoubleSpinBox *  sbCurrrentCutoffCharge= new QDoubleSpinBox();
        sbCurrrentCutoffCharge->setValue(parms.currentCutoffCharge);
        ui->tableWidget->setCellWidget(i,index++,sbCurrrentCutoffCharge);

        QDoubleSpinBox *  sbCurrrentCutoffDischarge = new QDoubleSpinBox();
        sbCurrrentCutoffDischarge->setValue(parms.currentCutoffDischarge);
        ui->tableWidget->setCellWidget(i,index++,sbCurrrentCutoffDischarge);

        QDoubleSpinBox *  sbReportingFrequency = new QDoubleSpinBox();
        sbReportingFrequency->setDecimals(1);
        sbReportingFrequency->setValue(parms.reportingFrequency);
        ui->tableWidget->setCellWidget(i,index++,sbReportingFrequency);

        QDoubleSpinBox * sbCurrentSetpoint = new QDoubleSpinBox();
        sbCurrentSetpoint->setValue(parms.currentSetpoint);
        ui->tableWidget->setCellWidget(i,index++,sbCurrentSetpoint);

        QDoubleSpinBox *  sbCapacity = new QDoubleSpinBox();
        sbCapacity->setMaximum(9999999.9f);
        sbCapacity->setValue(capacity);
        ui->tableWidget->setCellWidget(i, index++, sbCapacity);

        bar.setValue(i);
        QApplication::processEvents();
    }

    onSaveProject();
}

void wizardPageThree::onSaveProject() {

    QFile f( projectName + ".blp" );

    if (f.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
    {
        QTextStream data( &f);
        QStringList strList;

        for(int d = 0; d < ui->tableWidget->columnCount(); d++) {
            strList << " " + ui->tableWidget->horizontalHeaderItem(d)->data(Qt::DisplayRole).toString() + " ";
        }

        data << strList.join(",") + "\n";

        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                switch(c) {
                case 0:
                    strList << " "+qobject_cast<QLabel*>(ui->tableWidget->cellWidget( r, c ))->text()+" ";
                    break;
                case 1:
                case 2:
                    strList << " "+qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget( r, c ))->text()+" ";
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                    strList << " "+qobject_cast<QDoubleSpinBox*>(ui->tableWidget->cellWidget( r, c ))->text()+" ";
                    break;
                default:
                    break;
                }
            }
            data << strList.join( "," ) << endl;
        }
        f.close();
    }
}

void wizardPageThree::onDesignator(QString val)
{
    designator = val;
}

void wizardPageThree::onStartInd(QString val)
{
    startInd = val;
}

void wizardPageThree::onNumCells(int val)
{
    numCells = val;
}

void wizardPageThree::onTestParms(testParms val)
{
    parms = val;
}

void wizardPageThree::onNumCycles(int val)
{
    numCycles = val;
}

void wizardPageThree::onProjectName(QString val)
{
    projectName = val;
}

void wizardPageThree::onCapacity(double val)
{
    capacity = val;
}

void wizardPageThree::onRestTime(int val)
{
    restTime = val;
}
