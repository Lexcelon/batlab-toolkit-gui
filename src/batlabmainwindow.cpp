#include "batlabmainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "inputstringdialog.h"
#include <QScrollBar>
#include <cellmodulewidget.h>
#include <QSpacerItem>

BatlabMainWindow::BatlabMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setting up the gui with the same name in the forms directory
    ui->setupUi(this);
    this->showMaximized();
    tableWidget = ui->tableWidget;

    // Managing data from cells
    cellManager = new batlabCellManager();

    // Create the buttons we use in the gui
    connectToBatlabs = new QPushButton(QString("Connect to Batlab(s)"));
    test = new QPushButton(QString("Test"));
    loadProject = new QPushButton(QString("Load Project File"));
    newProjectWizard = new QPushButton(QString("New Project Wizard"));
    report = new QPushButton(QString("Debug GUI"));

    // Place the buttons in the button box in our gui
    ui->buttonBox->addButton(newProjectWizard,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(loadProject,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(connectToBatlabs,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(test,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(report,QDialogButtonBox::ActionRole);

    // TODO PLACEHOLDER TEXT WILL REMOVE
    ui->textBrowser->insertPlainText(QString(">> Welcome to Batlab!\n" ));
    ui->textBrowser->insertPlainText(QString(">> Click New Project Wizard or Load Project File to begin.\n" ));
    ui->textBrowser->insertPlainText(QString(">> Don't forget to plug in your Batlab(s) and click Connect to Batlab!\n" ));

    // Making the buttons functional
    connect(test, &QPushButton::clicked,
            this, &BatlabMainWindow::onTest);
    connect(report, &QPushButton::clicked,
            this, &BatlabMainWindow::onReport);
    connect(newProjectWizard,&QPushButton::clicked,
            this, &BatlabMainWindow::onNewProjectWizard);
    connect(connectToBatlabs, &QPushButton::clicked,
            this, &BatlabMainWindow::onGetBatlabNames);
    connect(loadProject, &QPushButton::clicked,
            this, &BatlabMainWindow::onLoadProject);
    connect(this, &BatlabMainWindow::emitUpdateText,
            this, &BatlabMainWindow::onUpdateText);
    connect(ui->testDataButton, &QPushButton::clicked,
            this, &BatlabMainWindow::onTestDataButton);
    connect(ui->processPackButton, &QPushButton::clicked,
            this, &BatlabMainWindow::onProcessPack);
    connect(cellManager, &batlabCellManager::emitPack,
            this, &BatlabMainWindow::onPackBuilt);

    connect(ui->newCellPlaylistButton, &QPushButton::clicked,
            this, &BatlabMainWindow::onNewCellPlaylistWizard);

    cellManager->test();
}

void BatlabMainWindow::onTest()
{
//    for (int i = 0; i < 200000; ++i) {
//        if (i%10 == 0) {

//            QString str = QString("Register #%1 - \n").arg(ui->textBrowser->verticalScrollBar()->maximum());
//            emit emitUpdateText(str);
//            QApplication::processEvents();
//        }
//    }
    if (!cellManager->getTestsRunning()) {
        if (batlabComObjects.size() > 0) {
            cellManager->onCreateTestPlan(batlabComObjects);
            cellManager->onStartTests();
        } else {
            QMessageBox::warning(this, "No Batlabs Connected", "Communications not established with any connected Batlabs!"
                                                               " Verify that connections are in place and then select "
                                                               "‘Connect to Batlab(s)’ option on the GUI Main Window.", QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this, "Tests are running!", "There are already tests running, please wait until tests are finished.", QMessageBox::Ok);
    }


}

void BatlabMainWindow::onReport()
{
       // For testing communications with batlab - BRING THIS BACK EVENTUALLY
       if (testObj == nullptr) {
           testObj = new batlabtest(this, batlabComObjects);
//           connect(testObj,SIGNAL(emitReadReg(int,int)),batlabComObjects.first(),SLOT(onReadReg(int,int)));
//           connect(testObj,SIGNAL(emitWriteReg(int,int,int)),batlabComObjects.first(),SLOT(onWriteReg(int,int,int)));
//           connect(testObj,SIGNAL(emitPrint(uchar,properties)),cellManager,SLOT(onPrintCell(uchar,properties)));
       }

       //Can move window around
       testObj->setModal(false);
           testObj->show();
}

BatlabMainWindow::~BatlabMainWindow()
{
    if (test) delete test;
    if (report) delete report;
    if (loadProject) delete loadProject;
    if (newProjectWizard) delete newProjectWizard;
    delete ui;
}

void BatlabMainWindow::onReceiveWriteCommand(int serialNumber, int nameSpace, int batlabRegister, int value)
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

    emit emitUpdateText(str);
}

void BatlabMainWindow::onReceiveReadCommand(int serialNumber, int nameSpace, int batlabRegister)
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

    emit emitUpdateText(str);
}

void BatlabMainWindow::onReceiveWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb)
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


    emit emitUpdateText(str);
}

void BatlabMainWindow::onReceiveReadResponse(int nameSpace, int batlabRegister, int lsb, int msb)
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


    emit emitUpdateText(str);
}

void BatlabMainWindow::onReceiveStream(int cell,int mode,int status,float temp, float current, float voltage)
{
    QString str = QString("STREAM PACKET: ");

    for (int i = 0; i < batlabComObjects.size(); ++i) {
        if (sender() == batlabComObjects[i]) {
            str += QString("Batlab #%1").arg(batlabComObjects[i]->getSerialNumber());
        }
    }

    str += QString("Cell #%1 - Mode: %2 - Status: %3 - Temp: %4 C - Current: %5 A - Voltage: %6 V \n")
            .arg(cell)
            .arg(mode)
            .arg(status)
            .arg(temp)
            .arg(current)
            .arg(voltage);
    qDebug() << str;

    emit emitUpdateText(str);
}

void BatlabMainWindow::onNewProjectWizard() {
    newTestScheduleWizard * a = new newTestScheduleWizard();
    connect(a, SIGNAL(emitFinished(QString)), this, SLOT(onLoadTest(QString)));
    a->onShow();
}

void BatlabMainWindow::onNewCellPlaylistWizard() {
    NewCellPlaylistWizard * wizard = new NewCellPlaylistWizard();
    wizard->setWizardStyle(QWizard::ModernStyle);
    wizard->show();
}

void BatlabMainWindow::onLoadTest(QString fileName)
{
    ui->tableWidget->clearContents();
    while( ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }
    while( ui->tableWidget->columnCount() > 0) {
        ui->tableWidget->removeColumn(0);
    }

    cellManager->clean();

    if (!fileName.endsWith(".blp",Qt::CaseInsensitive)) {
        fileName = fileName + ".blp";
    }

    cellManager->setProjectName(fileName.split(".").first());

    QFile f(fileName);

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
//            tempParms.restTime = QString(strList.at(index++)).toInt();
            tempParms.hightVoltageCutoff = QString(strList.at(index++)).toDouble();
            tempParms.lowVoltageCutoff = QString(strList.at(index++)).toDouble();
            tempParms.temperatureCutoffCharge = QString(strList.at(index++)).toDouble();
            tempParms.temperatureCutoffDischarge = QString(strList.at(index++)).toDouble();
//            tempParms.currentCutoffCharge = QString(strList.at(index++)).toDouble();
//            tempParms.currentCutoffDischarge = QString(strList.at(index++)).toDouble();
//            tempParms.reportingFrequency = QString(strList.at(index++)).toDouble();
            tempParms.chargeCurrentSetpoint = QString(strList.at(index++)).toDouble();
            tempParms.dischargeCurrentSetpoint = QString(strList.at(index++)).toDouble();
//            double cap = QString(strList.at(index++)).toDouble();

            int numberOfTests = numCycles * 2 + 1;
            if (ui->tableWidget->columnCount() - 1 < numberOfTests) {
                int colCount = ui->tableWidget->columnCount();
                for (int i = 0; i < (numberOfTests - colCount + 1); ++i) {
                    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
                }
            }

            cellManager->onNewCell(cellname,tempParms,numCycles);

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
                this, &BatlabMainWindow::onFinishedTests);
    }
}

void BatlabMainWindow::onLoadProject()
{
    QFileDialog dialog;
    dialog.setDefaultSuffix(QString(".blp"));

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if  (!fileNames.isEmpty())
        onLoadTest(fileNames.first());
}

void BatlabMainWindow::onTestDataButton()
{
    QFileDialog dialog;
    dialog.setDefaultSuffix(QString(".bld"));

    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    TestData *testData = new TestData;
    testData->readTestData(fileNames.first());

    ui->voltageGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_VOLTAGE);
    ui->currentGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_CURRENT);
    ui->temperatureGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_TEMPERATURE);
}

void BatlabMainWindow::onProcessPack()
{
    int numberOfModules = ui->numberOfModulesSpinbox->value();

    int numberOfCellsPerModule = ui->cellsPerModuleSpinbox->value();
    if ((cellManager->getCellList().size() < (numberOfCellsPerModule * numberOfModules))) {
        QMessageBox::critical(this, "Not enough cells.",
                              QString("You do not have enough cells to make %1 modules with %2 cells per module.").arg(numberOfModules).arg(numberOfCellsPerModule), QMessageBox::Ok);
        return;
    } else {
        cellManager->onSetNumberOfCellsPerModule(numberOfCellsPerModule);
        cellManager->onSetNumberOfModules(numberOfModules);
        cellManager->onSetVoltageCoefficient(ui->voltageCoefficientSpinbox->value());
        cellManager->onSetCurrentCoefficient(ui->currentCoefficientSpinbox->value());
        cellManager->onSetChargeCoefficient(ui->chargeCoefficientSpinbox->value());
        cellManager->onProcessCellData();
    }
}

void clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void BatlabMainWindow::onPackBuilt(QVector<QStringList> list)
{
    clearLayout(ui->packLayout);
    for (int i = 0; i < list.size(); ++i) {
        ui->packLayout->addWidget(new CellModuleWidget(i, list[i]));
        ui->packLayout->addSpacing(10);
    }
    ui->packLayout->addSpacerItem(new QSpacerItem(0 , 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void BatlabMainWindow::onFinishedTests(QString designator, int testNum)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (designator == ui->tableWidget->item(i,0)->text()) {
            ui->tableWidget->item(i,testNum+1)->setBackgroundColor(Qt::green);
            break;
        }
    }
}

void BatlabMainWindow::onUpdateText(QString str)
{
    static int i = 0;
    str = QString("%1: %2 ").arg(++i).arg(QDateTime::currentDateTime().toString()) + str;
    if (ui->textBrowser->verticalScrollBar()->value() >= (ui->textBrowser->verticalScrollBar()->maximum()-10)) {
        ui->textBrowser->insertPlainText(str);
        ui->textBrowser->moveCursor(QTextCursor::End);
    } else {
        ui->textBrowser->insertPlainText(str);
    }
}

void BatlabMainWindow::onConnectToBatlabs(QStringList names)
{
    qDebug() << names;
    for (int i = 0; i < names.size(); ++i) {
        bool connectBatlab = true;
        for (int j = 0; j < batlabComObjects.size(); ++j) {
            if (batlabComObjects[j]->getName() == names[i]) {
                connectBatlab = false;
            }
        }
        if (connectBatlab) {
            batlabComObjects.push_back(new batlabCom(names[i]));
            connect(batlabComObjects[i], &batlabCom::emitReadCommand,
                    this, &BatlabMainWindow::onReceiveReadCommand);
            connect(batlabComObjects[i], &batlabCom::emitWriteCommand,
                    this, &BatlabMainWindow::onReceiveWriteCommand);

            connect(batlabComObjects[i], &batlabCom::emitReadResponse,
                    this, &BatlabMainWindow::onReceiveReadResponse);
            connect(batlabComObjects[i], &batlabCom::emitWriteResponse,
                    this, &BatlabMainWindow::onReceiveWriteResponse);

            connect(batlabComObjects[i], &batlabCom::emitStream,
                    this, &BatlabMainWindow::onReceiveStream);
        }
    }
}

void BatlabMainWindow::onGetBatlabNames()
{
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    QStringList names;
    for (int i = 0; i< list.size(); ++i) {
        names.append(list[i].portName());
    }
    onConnectToBatlabs(names);
}
