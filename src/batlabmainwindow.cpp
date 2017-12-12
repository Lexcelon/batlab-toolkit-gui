#include "batlabmainwindow.h"
#include <QFileDialog>
#include "inputstringdialog.h"
#include <QScrollBar>
#include <cellmodulewidget.h>
#include <QSpacerItem>

BatlabMainWindow::BatlabMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Basic main window setup
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    this->resize(800, 600);
    this->showMaximized();
    this->setWindowTitle(tr("Batlab Toolkit GUI"));

    cellPlaylistButton = new QPushButton(tr("Cell Playlist"));
    batlabsButton = new QPushButton(tr("Batlabs"));
    liveViewButton = new QPushButton(tr("Live View"));
    resultsButton = new QPushButton(tr("Results"));

    tabButtonBox = new QDialogButtonBox;
    tabButtonBox->setOrientation(Qt::Vertical);
    tabButtonBox->addButton(cellPlaylistButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(batlabsButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(liveViewButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(resultsButton, QDialogButtonBox::ActionRole);

    mainStackedWidget = new QStackedWidget;

    cellPlaylistTabWidget = new QFrame;
    cellPlaylistTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    cellPlaylistTabWidget->setLineWidth(2);

    batlabsTabWidget = new QFrame;
    batlabsTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    batlabsTabWidget->setLineWidth(2);

    liveViewTabWidget = new QFrame;
    liveViewTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    liveViewTabWidget->setLineWidth(2);

    resultsTabWidget = new QFrame;
    resultsTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    resultsTabWidget->setLineWidth(2);

    cellPlaylistNotLoadedWidget = new QWidget;
    cellPlaylistNotLoadedLayout = new QGridLayout;

    noCellPlaylistLoadedLabel = new QLabel(tr("No cell playlist is loaded. Create a new playlist or open an existing one."));
    newCellPlaylistButton = new QPushButton(tr("New Cell Playlist"));
    connect(newCellPlaylistButton, &QPushButton::clicked, this, &BatlabMainWindow::onNewCellPlaylistWizard);
    openCellPlaylistButton = new QPushButton(tr("Open Cell Playlist"));
    connect(openCellPlaylistButton, &QPushButton::clicked, this, &BatlabMainWindow::onLoadCellPlaylist);

    cellPlaylistNotLoadedLayout->addWidget(noCellPlaylistLoadedLabel, 1, 1, 1, 3, Qt::AlignCenter);
    cellPlaylistNotLoadedLayout->addWidget(newCellPlaylistButton, 3, 1, Qt::AlignCenter);
    cellPlaylistNotLoadedLayout->addWidget(openCellPlaylistButton, 3, 3, Qt::AlignCenter);
    cellPlaylistNotLoadedLayout->setColumnStretch(0, 16);
    cellPlaylistNotLoadedLayout->setColumnStretch(2, 1);
    cellPlaylistNotLoadedLayout->setColumnStretch(4, 16);
    cellPlaylistNotLoadedLayout->setRowStretch(0, 6);
    cellPlaylistNotLoadedLayout->setRowStretch(2, 1);
    cellPlaylistNotLoadedLayout->setRowStretch(4, 16);
    cellPlaylistNotLoadedWidget->setLayout(cellPlaylistNotLoadedLayout);

    cellPlaylistLoadedWidget = new QWidget;
    cellPlaylistLoadedLayout = new QGridLayout;



    cellPlaylistStackedWidget = new QStackedWidget;
    cellPlaylistStackedWidget->addWidget(cellPlaylistNotLoadedWidget);
    cellPlaylistStackedWidget->addWidget(cellPlaylistLoadedWidget);

    cellPlaylistTabLayout = new QGridLayout;
    cellPlaylistTabLayout->addWidget(cellPlaylistStackedWidget, 0, 0);
    cellPlaylistTabWidget->setLayout(cellPlaylistTabLayout);

    textBrowser = new QTextBrowser;
    textBrowser->insertPlainText(QString(">> Welcome to Batlab Toolkit GUI\n" ));

    liveViewTabLayout = new QGridLayout;
    liveViewTabLayout->addWidget(textBrowser, 0, 0, Qt::AlignTop);
    liveViewTabWidget->setLayout(liveViewTabLayout);

    mainStackedWidget->addWidget(cellPlaylistTabWidget);
    mainStackedWidget->addWidget(batlabsTabWidget);
    mainStackedWidget->addWidget(liveViewTabWidget);
    mainStackedWidget->addWidget(resultsTabWidget);

    connect(cellPlaylistButton, &QPushButton::clicked, this, [this]{ mainStackedWidget->setCurrentWidget(cellPlaylistTabWidget); });
    connect(batlabsButton, &QPushButton::clicked, this, [this]{ mainStackedWidget->setCurrentWidget(batlabsTabWidget); });
    connect(liveViewButton, &QPushButton::clicked, this, [this]{ mainStackedWidget->setCurrentWidget(liveViewTabWidget); });
    connect(resultsButton, &QPushButton::clicked, this, [this]{ mainStackedWidget->setCurrentWidget(resultsTabWidget); });

    testCellsTabLayout = new QGridLayout;
    testCellsTabLayout->addWidget(tabButtonBox, 0, 0);
    testCellsTabLayout->addWidget(mainStackedWidget, 0, 1);

    testCellsTab = new QWidget;
    testCellsTab->setLayout(testCellsTabLayout);

    configurePackTabLayout = new QHBoxLayout;

    configurePackTab = new QWidget;
    configurePackTab->setLayout(configurePackTabLayout);

    mainTabWidget = new QTabWidget;
    mainTabWidget->addTab(testCellsTab, tr("Test Cells"));
    mainTabWidget->addTab(configurePackTab, tr("Configure Pack"));

    centralWidgetLayout = new QGridLayout;
    centralWidgetLayout->addWidget(mainTabWidget);
    centralWidget->setLayout(centralWidgetLayout);

    connect(this, &BatlabMainWindow::emitUpdateText,
            this, &BatlabMainWindow::onUpdateText);

    createActions();
    createMenus();

    statusBar()->showMessage(tr("Welcome to Batlab Toolkit GUI"));

    // Managing data from cells
    cellManager = new batlabCellManager;

    // Check for updates when the program opens and only display anything if updates are available
    updaterController->start(QtAutoUpdater::UpdateController::InfoLevel);
}

void BatlabMainWindow::createActions()
{
    newCellPlaylistAct = new QAction(tr("&New Cell Playlist"), this);
    newCellPlaylistAct->setShortcuts(QKeySequence::New);
    newCellPlaylistAct->setStatusTip(tr("Create a new cell playlist"));
    connect(newCellPlaylistAct, &QAction::triggered, this, &BatlabMainWindow::newCellPlaylist);

    openCellPlaylistAct = new QAction(tr("&Open Cell Playlist"), this);
    openCellPlaylistAct->setShortcuts(QKeySequence::Open);
    openCellPlaylistAct->setStatusTip(tr("Open an existing cell playlist"));
    connect(openCellPlaylistAct, &QAction::triggered, this, &BatlabMainWindow::openCellPlaylist);

    exitBatlabToolkitGUIAct = new QAction(tr("Exit"), this);
    exitBatlabToolkitGUIAct->setShortcuts(QKeySequence::Close);
    exitBatlabToolkitGUIAct->setStatusTip(tr("Close Batlab Toolkit GUI"));
    connect(exitBatlabToolkitGUIAct, &QAction::triggered, this, &BatlabMainWindow::exitBatlabToolkitGUI);

    debugBatlabAct = new QAction(tr("Debug Batlab"), this);
    debugBatlabAct->setStatusTip(tr("Debug a Batlab by reading and writing registers"));
    connect(debugBatlabAct, &QAction::triggered, this, &BatlabMainWindow::debugBatlab);

    aboutBatlabToolkitGUIAct = new QAction(tr("About Batlab Toolkit GUI"), this);
    aboutBatlabToolkitGUIAct->setStatusTip(tr("Information about the Batlab Toolkit GUI program"));
    connect(aboutBatlabToolkitGUIAct, &QAction::triggered, this, &BatlabMainWindow::aboutBatlabToolkitGUI);

    updaterController = new QtAutoUpdater::UpdateController("maintenancetool", this, qApp);
    updaterController->setDetailedUpdateInfo(true);
    checkForUpdatesAct = updaterController->createUpdateAction(this);
    checkForUpdatesAct->setIconVisibleInMenu(true);
}

void BatlabMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newCellPlaylistAct);
    fileMenu->addAction(openCellPlaylistAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitBatlabToolkitGUIAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(debugBatlabAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutBatlabToolkitGUIAct);
    helpMenu->addSeparator();
    helpMenu->addAction(checkForUpdatesAct);
}

void BatlabMainWindow::newCellPlaylist()
{
    onNewCellPlaylistWizard();
}

void BatlabMainWindow::openCellPlaylist()
{
    onLoadProject();
}

void BatlabMainWindow::exitBatlabToolkitGUI()
{
    this->close();
}

void BatlabMainWindow::debugBatlab()
{
    // For testing communications with batlab - TODO BRING THIS BACK EVENTUALLY
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

void BatlabMainWindow::checkForBatlabFirmwareUpdates()
{
    // TODO
}

void BatlabMainWindow::aboutBatlabToolkitGUI()
{
    QString msgText = QString("<p>Batlab Toolkit GUI, Version %1"
                              "<p>© Lexcelon, LLC %2"
                              "<hr>"
                              "<p>Batlab Toolkit GUI is provided under the GPL license."
                              "<p>Source code is available on <a href=\"https://www.github.com/lexcelon/batlab-toolkit-gui\">GitHub</a>."
                              "<p>Documentation is available on the <a href=\"https://www.lexcelon.com/resources/\">resources</a> page on our website."
                              "<p>Please <a href=\"https://www.lexcelon.com\">visit our website</a>"
                              " or <a href=\"mailto:support@lexcelon.com\">contact us</a> for more information."
                              "<hr>"
                              "<p>The Batlab is made possible through the support and participation of our backers and customers. Thank you!"
                              ).arg(BATLAB_TOOLKIT_GUI_VERSION, QDate::currentDate().toString("yyyy"));
    QMessageBox::information(this, tr("About Batlab Toolkit GUI"), msgText);
}

void BatlabMainWindow::onTest()
{
//    for (int i = 0; i < 200000; ++i) {
//        if (i%10 == 0) {

//            QString str = QString("Register #%1 - \n").arg(textBrowser->verticalScrollBar()->maximum());
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

BatlabMainWindow::~BatlabMainWindow()
{

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

void BatlabMainWindow::onNewCellPlaylistWizard() {
    NewCellPlaylistWizard * wizard = new NewCellPlaylistWizard();
    wizard->setWizardStyle(QWizard::ModernStyle);
    wizard->show();
}

void BatlabMainWindow::onLoadCellPlaylist() {

}

void BatlabMainWindow::onLoadTest(QString fileName)
{
//    ui->tableWidget->clearContents();
//    while( ui->tableWidget->rowCount() > 0) {
//        ui->tableWidget->removeRow(0);
//    }
//    while( ui->tableWidget->columnCount() > 0) {
//        ui->tableWidget->removeColumn(0);
//    }

//    cellManager->clean();

//    if (!fileName.endsWith(".blp",Qt::CaseInsensitive)) {
//        fileName = fileName + ".blp";
//    }

//    cellManager->setProjectName(fileName.split(".").first());

//    QFile f(fileName);

//    QVector<QString> labels;

//    if (f.open(QFile::ReadOnly)){

//        if (!f.atEnd()) {
//           QByteArray str = f.readLine();

//           qDebug() << str.split(',');

//           for (int i = 0; i < str.split(',').size(); ++i) {
//               labels.append(str.split(',').at(i));
//           }
//        }

//        while (!f.atEnd()) {
//            testParms tempParms;
//            int index = 0;

//            QByteArray str = f.readLine();
//            QList<QByteArray> strList = str.split(',');
//            QString cellname = strList.at(index++);
//            int numCycles = QString(strList.at(index++)).toInt();
////            tempParms.restTime = QString(strList.at(index++)).toInt();
//            tempParms.hightVoltageCutoff = QString(strList.at(index++)).toDouble();
//            tempParms.lowVoltageCutoff = QString(strList.at(index++)).toDouble();
//            tempParms.temperatureCutoffCharge = QString(strList.at(index++)).toDouble();
//            tempParms.temperatureCutoffDischarge = QString(strList.at(index++)).toDouble();
////            tempParms.currentCutoffCharge = QString(strList.at(index++)).toDouble();
////            tempParms.currentCutoffDischarge = QString(strList.at(index++)).toDouble();
////            tempParms.reportingFrequency = QString(strList.at(index++)).toDouble();
//            tempParms.chargeCurrentSetpoint = QString(strList.at(index++)).toDouble();
//            tempParms.dischargeCurrentSetpoint = QString(strList.at(index++)).toDouble();
////            double cap = QString(strList.at(index++)).toDouble();

//            int numberOfTests = numCycles * 2 + 1;
//            if (ui->tableWidget->columnCount() - 1 < numberOfTests) {
//                int colCount = ui->tableWidget->columnCount();
//                for (int i = 0; i < (numberOfTests - colCount + 1); ++i) {
//                    ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
//                }
//            }

//            cellManager->onNewCell(cellname,tempParms,numCycles);

//            QVector<int> *cellTests = cellManager->onGetCell(cellname)->getTests();
//            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//            QTableWidgetItem * item = new QTableWidgetItem(cellname);
//            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,item);
//            for (int j = 0; j < cellTests->size(); j++) {
//                QString testString;
//                if (cellTests->at(j) == MODE_CHARGE) {
//                    testString = "CHARGE";
//                } else {
//                    testString = "DISCHARGE";
//                }
//                QTableWidgetItem * testItem = new QTableWidgetItem(testString);
//                testItem->setBackgroundColor(Qt::red);
//                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,j+1,testItem);
//            }

//        }

//    }
//    for (int i = 0; i < cellManager->getCellList().size(); ++i) {
//        connect(cellManager->getCellList()[i], &batlabCell::updateUI,
//                this, &BatlabMainWindow::onFinishedTests);
//    }
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

//    ui->voltageGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_VOLTAGE);
//    ui->currentGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_CURRENT);
//    ui->temperatureGraph->onLineGraph(testData->getTestData().TIME,testData->getTestData().REG_TEMPERATURE);
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

void BatlabMainWindow::onFinishedTests(QString designator, int testNum)
{
//    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
//        if (designator == ui->tableWidget->item(i,0)->text()) {
//            ui->tableWidget->item(i,testNum+1)->setBackgroundColor(Qt::green);
//            break;
//        }
//    }
}

void BatlabMainWindow::onUpdateText(QString str)
{
    static int i = 0;
    str = QString("%1: %2 ").arg(++i).arg(QDateTime::currentDateTime().toString()) + str;
    if (textBrowser->verticalScrollBar()->value() >= (textBrowser->verticalScrollBar()->maximum()-10)) {
        textBrowser->insertPlainText(str);
        textBrowser->moveCursor(QTextCursor::End);
    } else {
        textBrowser->insertPlainText(str);
    }
}

// Returns QStringList of names of available comm ports.
QStringList BatlabMainWindow::getAvailCommPortNames() {

    QList<QSerialPortInfo> availCommPorts = QSerialPortInfo::availablePorts();
    QStringList availCommPortNames;

    for (int i = 0; i < availCommPorts.size(); ++i) {
        availCommPortNames.append(availCommPorts[i].portName());
    }

    return availCommPortNames;

}

void BatlabMainWindow::makeBatlabConnections(QStringList availCommPortNames) {

    // If the connected Batlab has a port name that matches any of the available port names, ignore it
    // (it does not need to be re-connected). If no match is found, then make the connection.

    for (int i = 0; i < availCommPortNames.size(); ++i) {

        bool connectBatlab = true;
        for (int j = 0; j < batlabComObjects.size(); ++j) {

            if (batlabComObjects[j]->getName() == availCommPortNames[i]) {
                connectBatlab = false;
            }
    }

        if (connectBatlab) {

            batlabComObjects.push_back(new batlabCom(availCommPortNames[i]));
            connect(batlabComObjects[i], &batlabCom::emitBatlabDisconnect, this, &BatlabMainWindow::removeBatlabConnection);

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

void BatlabMainWindow::updateBatlabConnections() {

    //Updates Batlab connections (if any need to be made).
    makeBatlabConnections(getAvailCommPortNames());

    return;
}

// Disconnecting a Batlab unit drives the Batlab Comm class to send its port name to this function
// so that it may be removed from the list of connected Batlab units.
void BatlabMainWindow::removeBatlabConnection(QString batlabUnitPortName) {

    bool foundIndexToDelete = false;
    int currentIndex = 0;

    while (!foundIndexToDelete && (currentIndex < batlabComObjects.size())) {
        if (batlabComObjects[currentIndex]->getName() != batlabUnitPortName) {
                currentIndex++;
            }

        else {
            foundIndexToDelete = true;
        }
    }

    if (foundIndexToDelete) {
        batlabComObjects.removeAt(currentIndex);
    }

    return;
}

//void BatlabMainWindow::onConnectToBatlabs(QStringList names)
//{
//    qDebug() << names;
//    for (int i = 0; i < names.size(); ++i) {
//        bool connectBatlab = true;
//        for (int j = 0; j < batlabComObjects.size(); ++j) {
//            if (batlabComObjects[j]->getName() == names[i]) {
//                connectBatlab = false;
//            }
//        }
//        if (connectBatlab) {
//            batlabComObjects.push_back(new batlabCom(names[i]));
//            connect(batlabComObjects[i], &batlabCom::emitReadCommand,
//                    this, &BatlabMainWindow::onReceiveReadCommand);
//            connect(batlabComObjects[i], &batlabCom::emitWriteCommand,
//                    this, &BatlabMainWindow::onReceiveWriteCommand);

//            connect(batlabComObjects[i], &batlabCom::emitReadResponse,
//                    this, &BatlabMainWindow::onReceiveReadResponse);
//            connect(batlabComObjects[i], &batlabCom::emitWriteResponse,
//                    this, &BatlabMainWindow::onReceiveWriteResponse);

//            connect(batlabComObjects[i], &batlabCom::emitStream,
//                    this, &BatlabMainWindow::onReceiveStream);
//        }
//    }
//}

//void BatlabMainWindow::onGetBatlabNames()
//{
//    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
//    QStringList names;
//    for (int i = 0; i< list.size(); ++i) {
//        names.append(list[i].portName());
//    }
//    onConnectToBatlabs(names);
//}
