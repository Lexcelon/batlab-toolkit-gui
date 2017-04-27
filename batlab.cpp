#include "batlab.h"
#include "ui_batlab.h"
#include <QFileDialog>
#include "inputStringDialog.h"

Batlab::Batlab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Batlab)
{
    // Setting up the gui with the same name in the forms directory
    ui->setupUi(this);
    tableWidget = ui->tableWidget;

    // Managing data from cells
    cellManager = new batlabCellManager();

    // Create the buttons we use in the gui
    connectToBatlabs = new QPushButton(QString("Connect to Batlab(s)"));
    test = new QPushButton(QString("Test"));
    loadProject = new QPushButton(QString("Load Project File"));
    options = new QPushButton(QString("Options"));
    newProjectWizard = new QPushButton(QString("New Project Wizard"));
    report = new QPushButton(QString("Report"));
    exit = new QPushButton(QString("Exit"));

    // Place the buttons in the button box in our gui
    ui->buttonBox->addButton(newProjectWizard,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(loadProject,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(connectToBatlabs,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(test,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(report,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(options,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(exit,QDialogButtonBox::ActionRole);

    // PLACEHOLDER TEXT WILL REMOVE
    ui->textBrowser->insertPlainText(QString(">> Welcome to Batlab!\n" ));
    ui->textBrowser->insertPlainText(QString(">> Click New Project Wizard or Load Project File to begin.\n" ));
    ui->textBrowser->insertPlainText(QString(">> Don't forget to plug in your Batlab(s) and click Connect to Batlab!\n" ));

    // Making the buttons functional
    connect(exit, &QPushButton::clicked,
            this, &Batlab::close);
    connect(test, &QPushButton::clicked,
            this, &Batlab::onTest);
    connect(newProjectWizard,&QPushButton::clicked,
            this, &Batlab::onNewProjectWizard);
    connect(connectToBatlabs, &QPushButton::clicked,
            this, &Batlab::onGetBatlabNames);
    connect(loadProject, &QPushButton::clicked,
            this, &Batlab::onLoadProject);
}

void Batlab::onTest()
{
    if (batlabComObjects.size() > 0) {
        cellManager->onCreateTestPlan(batlabComObjects);
        cellManager->onStartTests();
    } else {
        QMessageBox::warning(this, "No Batlabs Connected", "There are not any connected Batlabs! Make sure everything's hooked up right and try again.", QMessageBox::Ok);
    }

//   // For testing communications with batlab
//   if (testObj == nullptr) {
//       testObj = new batlabtest();
//       connect(testObj,SIGNAL(emitReadReg(int,int,vals)),com,SLOT(onReadReg(int,int,vals)));
//       connect(testObj,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
//       connect(testObj,SIGNAL(emitPrint(uchar,properties)),cellManager,SLOT(onPrintCell(uchar,properties)));
//   }

//   //Can move window around
//   testObj->setModal(false);
//   testObj->show();
}

Batlab::~Batlab()
{
    if (exit) delete exit;
    if (test) delete test;
    if (options) delete options;
    if (report) delete report;
    if (loadProject) delete loadProject;
    if (newProjectWizard) delete newProjectWizard;
    delete ui;
}

void Batlab::onReceiveWriteCommand(int serialNumber, int nameSpace, int batlabRegister, int value)
{
    QString str;
    str += QString("WRITE: Batlab #%1 - ").arg(serialNumber);
    if (nameSpace >=0 && nameSpace < 4) {
        str += QString("Cell #%1 - ").arg(nameSpace);
    } else if (nameSpace == 4) {
        str += QString("Unit Namespace - ");
    } else {
        str += QString("If you see this, then we have problems.");
    }

    str += QString("Register #%1 - ").arg(batlabRegister);
    str += QString("Value = %1 \n").arg(value);

    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveReadCommand(int serialNumber, int nameSpace, int batlabRegister)
{
    QString str;
    str += QString("READ: Batlab #%1 - ").arg(serialNumber);
    if (nameSpace >=0 && nameSpace < 4) {
        str += QString("Cell #%1 - ").arg(nameSpace);
    } else if (nameSpace == 4) {
        str += QString("Unit Namespace - ");
    } else {
        str += QString("If you see this, then we have problems.");
    }

    str += QString("Register #%1 - \n").arg(batlabRegister);
    qDebug() << str;
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb)
{
    QString str = QString("WRITE RESPONSE: ");
    if (nameSpace >=0 && nameSpace < 4) {
        str += QString("Cell #%1 - ").arg(nameSpace);
    } else if (nameSpace == 4) {
        str += QString("Unit Namespace - ");
    } else {
        str += QString("If you see this, then we have problems.");
    }

    str += QString("Register #%1 - ").arg(batlabRegister);
    str += QString("MSB: %1 - ").arg(msb,4,16);
    str += QString("LSB: %1 \n").arg(lsb,4,16);
qDebug() << str;
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb)
{
    QString str = QString("READ RESPONSE: ");
    if (nameSpace >=0 && nameSpace < 4) {
        str += QString("Cell #%1 - ").arg(nameSpace);
    } else if (nameSpace == 4) {
        str += QString("Unit Namespace - ");
    } else {
        str += QString("If you see this, then we have problems.");
    }

    str += QString("Register #%1 - ").arg(batlabRegister);
    str += QString("MSB: %1 - ").arg(msb,4,16);
    str += QString("LSB: %1 \n").arg(lsb,4,16);
qDebug() << str;
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveStream(int cell,int mode,int status,float temp, float current, float voltage)
{
    QString str = QString("STREAM PACKET: ");

    for (int i = 0; i < batlabComObjects.size(); ++i) {
        if (sender() == batlabComObjects[i]) {
            str += QString("Batlab #%1").arg(batlabComObjects[i]->getSerialNumber());
        }
    }

    str += QString("Cell #%1 - Mode: %2 - Status: %3 - Temp: %4 Units- Current: %5 A - Voltage: %6 V \n")
            .arg(cell)
            .arg(mode)
            .arg(status)
            .arg(temp)
            .arg(current)
            .arg(voltage);
qDebug() << str;
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onAddTests()
{

}

void Batlab::onNewProjectWizard() {
    batlabWizard * a = new batlabWizard();
    connect(a, SIGNAL(emitFinished(QString)), this, SLOT(onLoadTest(QString)));
    a->onShow();
}

void Batlab::onLoadTest(QString name)
{
    ui->tableWidget->clearContents();
    while( ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }
    while( ui->tableWidget->columnCount() > 0) {
        ui->tableWidget->removeColumn(0);
    }

    cellManager->clean();

    if (!name.endsWith(".blp",Qt::CaseInsensitive)) {
        name = name + ".blp";
    }

    cellManager->setProjectName(name.split(".").first());

    QFile f(name);

    QVector<QString> labels;

    if (f.open(QFile::ReadOnly)){

        if (!f.atEnd()) {
           QByteArray str = f.readLine();

           qDebug() << str.split(',');

           for (int i = 0; i < str.split(',').size(); ++i) {
               labels.append(str.split(',').at(i));
           }
        }

        while (!f.atEnd()) {
            testParms tempParms;
            int index = 0;

            QByteArray str = f.readLine();
            QList<QByteArray> strList = str.split(',');
            QString cellname = strList.at(index++);
            int numCycles = QString(strList.at(index++)).toInt();
            tempParms.restTime = QString(strList.at(index++)).toInt();
            tempParms.hightVoltageCutoff = QString(strList.at(index++)).toDouble();
            tempParms.lowVoltageCutoff = QString(strList.at(index++)).toDouble();
            tempParms.temperatureCutoffCharge = QString(strList.at(index++)).toDouble();
            tempParms.temperatureCutoffDischarge = QString(strList.at(index++)).toDouble();
            tempParms.currentCutoffCharge = QString(strList.at(index++)).toDouble();
            tempParms.currentCutoffDischarge = QString(strList.at(index++)).toDouble();
            tempParms.reportingFrequency = QString(strList.at(index++)).toDouble();
            tempParms.currentSetpoint = QString(strList.at(index++)).toDouble();
            double cap = QString(strList.at(index++)).toDouble();

            int numberOfTests = numCycles * 2 + 1;
            if (ui->tableWidget->columnCount() - 1 < numberOfTests) {
                int colCount = ui->tableWidget->columnCount();
                for (int i = 0; i < (numberOfTests - colCount + 1); ++i) {
                    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
                }
            }

            cellManager->onNewCell(cellname,tempParms,cap,numCycles);

            QVector<int> *cellTests = cellManager->onGetCell(cellname)->getTests();
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            QTableWidgetItem * item = new QTableWidgetItem(cellname);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,item);
            for (int j = 0; j < cellTests->size(); j++) {
                QString testString;
                if (cellTests->at(j) == MODE_CHARGE) {
                    testString = "CHARGE";
                } else {
                    testString = "DISCHARGE";
                }
                QTableWidgetItem * testItem = new QTableWidgetItem(testString);
                testItem->setBackgroundColor(Qt::red);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,j+1,testItem);
            }

        }

    }
    for (int i = 0; i < cellManager->getCellList().size(); ++i) {
        connect(cellManager->getCellList()[i], &batlabCell::updateUI,
                this, &Batlab::onFinishedTests);
    }
}

void Batlab::onLoadProject()
{
    QFileDialog dialog;
    dialog.setDefaultSuffix(QString(".blp"));

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    onLoadTest(fileNames.first());
}

void Batlab::onFinishedTests(QString designator, int testNum)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (designator == ui->tableWidget->item(i,0)->text()) {
            ui->tableWidget->item(i,testNum+1)->setBackgroundColor(Qt::green);
            break;
        }
    }
}

void Batlab::onConnectToBatlabs(QStringList names)
{
    qDebug() << names;
    for (int i = 0; i < names.size(); ++i) {
        batlabComObjects.push_back(new batlabCom(names[i]));
        connect(batlabComObjects[i], &batlabCom::emitReadCommand,
                this, &Batlab::onReceiveReadCommand);
        connect(batlabComObjects[i], &batlabCom::emitWriteCommand,
                this, &Batlab::onReceiveWriteCommand);

        connect(batlabComObjects[i], &batlabCom::emitReadResponse,
                this, &Batlab::onReceiveReadResponse);
        connect(batlabComObjects[i], &batlabCom::emitWriteResponse,
                this, &Batlab::onReceiveWriteResponse);

        connect(batlabComObjects[i], &batlabCom::emitStream,
                this, &Batlab::onReceiveStream);
    }
}

void Batlab::onGetBatlabNames()
{
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    QStringList names;
    for (int i = 0; i< list.size(); ++i) {
        names.append(list[i].portName());
    }
    onConnectToBatlabs(names);
}
