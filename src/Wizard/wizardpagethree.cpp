#include "wizardpagethree.h"
#include "ui_wizardpagethree.h"
#include <QFileDialog>
#include <QMessageBox>
wizardPageThree::wizardPageThree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageThree)
{
    ui->setupUi(this);
    ui->tableWidget->resizeColumnsToContents();
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

    onGetFilepath();

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

//        QSpinBox * sbRestTime = new QSpinBox();
//        sbRestTime->setMaximum(0xFFFF);
//        sbRestTime->setValue(restTime);
//        ui->tableWidget->setCellWidget(i,index++,sbRestTime);

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

//        QDoubleSpinBox *  sbCurrrentCutoffCharge= new QDoubleSpinBox();
//        sbCurrrentCutoffCharge->setValue(parms.currentCutoffCharge);
//        ui->tableWidget->setCellWidget(i,index++,sbCurrrentCutoffCharge);

//        QDoubleSpinBox *  sbCurrrentCutoffDischarge = new QDoubleSpinBox();
//        sbCurrrentCutoffDischarge->setValue(parms.currentCutoffDischarge);
//        ui->tableWidget->setCellWidget(i,index++,sbCurrrentCutoffDischarge);

//        QDoubleSpinBox *  sbReportingFrequency = new QDoubleSpinBox();
//        sbReportingFrequency->setDecimals(1);
//        sbReportingFrequency->setValue(parms.reportingFrequency);
//        ui->tableWidget->setCellWidget(i,index++,sbReportingFrequency);

        QDoubleSpinBox * sbChargeCurrentSetpoint = new QDoubleSpinBox();
        sbChargeCurrentSetpoint->setValue(parms.chargeCurrentSetpoint);
        ui->tableWidget->setCellWidget(i,index++,sbChargeCurrentSetpoint);


        QDoubleSpinBox * sbDischargeCurrentSetpoint = new QDoubleSpinBox();
        sbDischargeCurrentSetpoint->setValue(parms.dischargeCurrentSetpoint);
        ui->tableWidget->setCellWidget(i,index++,sbDischargeCurrentSetpoint);

        QDoubleSpinBox *  sbCapacity = new QDoubleSpinBox();
        sbCapacity->setMaximum(9999999.9f);
        sbCapacity->setValue(capacity);
        ui->tableWidget->setCellWidget(i, index++, sbCapacity);

        bar.setValue(i);
        QApplication::processEvents();
    }

//    onGetFilepath();
//    onSaveProject();
}
QString wizardPageThree::onGetFilepath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory to save your project file."),
                                                "C:/",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    qDebug() << projectName;
    projectName = dir + projectName;
    return dir;
}


void wizardPageThree::onSaveProject()
{
    if (!projectName.endsWith(".blp")) {
        projectName.append(".blp");
    }
    QFile f( projectName );

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
                    strList << " "+qobject_cast<QSpinBox*>(ui->tableWidget->cellWidget( r, c ))->text()+" ";
                    break;
                case 2:
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
    } else {
        QMessageBox::warning(this, "Access is denied!", "The file failed to open. Try running as administrator to write to that location. "
                                                        "File was saved to executable directory.", QMessageBox::Ok);
        QStringList list = projectName.split("/");
        projectName = list.last();
        this->onSaveProject();
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
