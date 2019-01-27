#include "batlabmainwindow.h"
#include <QFileDialog>
#include <QScrollBar>
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

    connect(BatlabLogger::instance(), &BatlabLogger::qtLogMessageReceived, this, &BatlabMainWindow::processQtLogMessage);

    // BatlabManager keeps track of connected Batlabs and handles state by itself. Only passes updates to BatlabMainWindow so that they can be drawn.
    batlabManager = new BatlabManager;
    connect(batlabManager, &BatlabManager::batlabInfoUpdated, this, &BatlabMainWindow::redrawBatlabInfo);

    // Setup the UI
    initializeMainWindowUI();

    connect(this, &BatlabMainWindow::emitUpdateText,
            this, &BatlabMainWindow::updatelogViewTextBrowser);

    createActions();
    createMenus();

    statusBar()->showMessage(tr("Welcome to Batlab Toolkit GUI!"));

    // Check for updates when the program opens and only display anything if updates are available
    // I have disabled this because it asks if maintenancetool.exe can make changes to your computer every time you open the program
    // Will reconsider in future especially if I can make it not intrusive. For now user can run "Check for updates"
    // updaterController->start(QtAutoUpdater::UpdateController::InfoLevel);
}

void BatlabMainWindow::initializeMainWindowUI()
{
    cellPlaylistButton = new QPushButton(tr("Cell Playlist"));
    batlabsButton = new QPushButton(tr("Batlabs"));
    logViewButton = new QPushButton(tr("Log"));
    resultsButton = new QPushButton(tr("Results"));

    tabButtonBox = new QDialogButtonBox;
    tabButtonBox->setOrientation(Qt::Vertical);
    tabButtonBox->addButton(cellPlaylistButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(batlabsButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(logViewButton, QDialogButtonBox::ActionRole);
    tabButtonBox->addButton(resultsButton, QDialogButtonBox::ActionRole);

    mainStackedWidget = new QStackedWidget;

    cellPlaylistTabWidget = new QFrame;
    cellPlaylistTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    cellPlaylistTabWidget->setLineWidth(2);

    batlabsTabWidget = new QFrame;
    batlabsTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    batlabsTabWidget->setLineWidth(2);
    batlabsTabWidget->setAutoFillBackground(true);

    batlabsTabLayout = new QVBoxLayout;
    batlabsTabWidget->setLayout(batlabsTabLayout);

    logViewTabWidget = new QFrame;
    logViewTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    logViewTabWidget->setLineWidth(2);

    resultsTabWidget = new QFrame;
    resultsTabWidget->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    resultsTabWidget->setLineWidth(2);

    resultsTabLayout = new QVBoxLayout;
    resultsTabWidget->setLayout(resultsTabLayout);

    cellPlaylistNotLoadedWidget = new QWidget;
    cellPlaylistNotLoadedLayout = new QGridLayout;

    noCellPlaylistLoadedLabel = new QLabel(tr("No cell playlist is loaded. Create a new playlist or open an existing one."));
    newCellPlaylistButton = new QPushButton(tr("New Cell Playlist"));
    connect(newCellPlaylistButton, &QPushButton::clicked, this, &BatlabMainWindow::showNewCellPlaylistWizard);
    openCellPlaylistButton = new QPushButton(tr("Open Cell Playlist"));
    connect(openCellPlaylistButton, &QPushButton::clicked, this, &BatlabMainWindow::openCellPlaylist);

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
    cellPlaylistLoadedWidget->setLayout(cellPlaylistLoadedLayout);

    cellPlaylistStackedWidget = new QStackedWidget;
    cellPlaylistStackedWidget->addWidget(cellPlaylistNotLoadedWidget);
    cellPlaylistStackedWidget->addWidget(cellPlaylistLoadedWidget);
    cellPlaylistStackedWidget->setCurrentWidget(cellPlaylistNotLoadedWidget);

    cellPlaylistTabLayout = new QGridLayout;
    cellPlaylistTabLayout->addWidget(cellPlaylistStackedWidget, 0, 0);
    cellPlaylistTabWidget->setLayout(cellPlaylistTabLayout);

    logViewTabLayout = new QVBoxLayout;

    logViewTextBrowser = new QTextBrowser;
    logViewTextBrowser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    updatelogViewTextBrowser("Welcome to Batlab Toolkit GUI!");

    logViewButtonLayout = new QHBoxLayout;
    logViewButtonLayout->setContentsMargins(0, 0, 0, 0);

    bool printDebugMessagesDefault = false;
    printDebugMessages = printDebugMessagesDefault;
    logViewPrintDebugCheckBox = new QCheckBox(tr("Print Debug Messages"));
    logViewPrintDebugCheckBox->setChecked(printDebugMessagesDefault);
    logViewPrintDebugCheckBox->setLayoutDirection(Qt::RightToLeft);
    logViewButtonLayout->addWidget(logViewPrintDebugCheckBox);
    connect(logViewPrintDebugCheckBox, &QCheckBox::toggled, this, &BatlabMainWindow::togglePrintDebugMessages);
    logViewButtonLayout->addStretch(10);
    logViewClearButton = new QPushButton(tr("Clear"));
    logViewButtonLayout->addWidget(logViewClearButton);
    connect(logViewClearButton, &QPushButton::clicked, logViewTextBrowser, &QTextBrowser::clear);
    logViewSaveButton = new QPushButton(tr("Save Output"));
    logViewButtonLayout->addWidget(logViewSaveButton);
    connect(logViewSaveButton, &QPushButton::clicked, this, &BatlabMainWindow::savelogView);

    logViewTabLayout->addWidget(logViewTextBrowser);
    logViewTabLayout->addLayout(logViewButtonLayout);
    logViewTabWidget->setLayout(logViewTabLayout);

    mainStackedWidget->addWidget(cellPlaylistTabWidget);
    mainStackedWidget->addWidget(batlabsTabWidget);
    mainStackedWidget->addWidget(logViewTabWidget);
    mainStackedWidget->addWidget(resultsTabWidget);

    mainStackedWidget->setCurrentWidget(cellPlaylistTabWidget);
    cellPlaylistButton->setEnabled(false);

    // Some fun functor syntax to pass arguments to the signal https://stackoverflow.com/a/22411267
    // You have to capture ''this'' and then access variables from there https://stackoverflow.com/questions/7895879/using-member-variable-in-lambda-capture-list-inside-a-member-function
    connect(cellPlaylistButton, &QPushButton::clicked, this, [this]{
        mainStackedWidget->setCurrentWidget(cellPlaylistTabWidget);
        cellPlaylistButton->setEnabled(false);
        batlabsButton->setEnabled(true);
        logViewButton->setEnabled(true);
        resultsButton->setEnabled(true);
    });
    connect(batlabsButton, &QPushButton::clicked, this, [this]{
        mainStackedWidget->setCurrentWidget(batlabsTabWidget);
        cellPlaylistButton->setEnabled(true);
        batlabsButton->setEnabled(false);
        logViewButton->setEnabled(true);
        resultsButton->setEnabled(true);
    });
    connect(logViewButton, &QPushButton::clicked, this, [this]{
        mainStackedWidget->setCurrentWidget(logViewTabWidget);
        cellPlaylistButton->setEnabled(true);
        batlabsButton->setEnabled(true);
        logViewButton->setEnabled(false);
        resultsButton->setEnabled(true);
    });
    connect(resultsButton, &QPushButton::clicked, this, [this]{
        mainStackedWidget->setCurrentWidget(resultsTabWidget);
        cellPlaylistButton->setEnabled(true);
        batlabsButton->setEnabled(true);
        logViewButton->setEnabled(true);
        resultsButton->setEnabled(false);
    });

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
//    mainTabWidget->addTab(configurePackTab, tr("Configure Pack"));

    centralWidgetLayout = new QGridLayout;
    centralWidgetLayout->addWidget(mainTabWidget);
    centralWidget->setLayout(centralWidgetLayout);
}

void BatlabMainWindow::togglePrintDebugMessages(bool value)
{
    printDebugMessages = value;
}

void BatlabMainWindow::savelogView()
{
    // TODO default path should be as is if no playlist loaded, should be in directory/named file for particular playlist if one loaded
    QString appLocalDataPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();

    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Log Output"),
                                                        tr("%1/logfile.txt").arg(appLocalDataPath),
                                                        tr("Text Files (*.txt);;All Files (*)"));

    if (saveFileName.isEmpty())
    {
        return;
    }

   QFile file(saveFileName);
   if (!file.open(QIODevice::WriteOnly))
   {
       QMessageBox::warning(this,
                            tr("Error"),
                            QString(tr("<nobr>File '%1'<br/>cannot be opened for writing.<br/><br/>"
                                       "The log output could <b>not</b> be saved!</nobr>"))
       .arg(saveFileName));
       return;
    }

    QTextStream stream(&file);
    stream << logViewTextBrowser->toPlainText();
    file.close();
}

// https://stackoverflow.com/questions/4272196/qt-remove-all-widgets-from-layout
void clearLayout(QLayout *layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (deleteWidgets) {
            if (QWidget* widget = item->widget()) {
                widget->deleteLater();
            }
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout, deleteWidgets);
        }
        delete item;
    }
}

// On Linux, when a Batlab is unplugged, the event loop for the whole application seems to pause and the Batlab does not show up as unplugged.
// This is lifted as soon as the keyboard or mouse do anything inside this window, so if you are moving the mouse while unplugging a Batlab there is no problem.
// Minor and weird enough bug that I am giving up on it. Seems like a Qt thing.
// http://www.qtforum.org/article/36406/events-not-called-unless-mouse-is-moving.html
// https://forum.qt.io/topic/8630/events-not-called-unless-mouse-is-moving/2
// https://bugreports.qt.io/browse/QTBUG-7728
void BatlabMainWindow::redrawBatlabInfo(QVector<batlabStatusInfo> infos, int latestFirmwareVersion)
{
    clearLayout(batlabsTabLayout, true);

    for (int i = 0; i < infos.size(); i++) {
        BatlabWidget *batlabWidget = new BatlabWidget(infos[i], latestFirmwareVersion);
        batlabsTabLayout->addWidget(batlabWidget);
        connect(batlabWidget, &BatlabWidget::firmwareUpdateRequested, batlabManager, &BatlabManager::processFirmwareFlashRequest);
    }

    batlabsTabLayout->addStretch();

    batlabsButton->setText("Batlabs (" + QString::number(infos.size()) + ")");
}

void BatlabMainWindow::redrawResultsInfo(QVector<cellResultsStatusInfo> infos)
{
    clearLayout(resultsTabLayout, true);

    ResultsWidget *resultsWidget = new ResultsWidget(infos);
    resultsTabLayout->addWidget(resultsWidget);
}

void BatlabMainWindow::createActions()
{
    newCellPlaylistAct = new QAction(tr("&New Cell Playlist"), this);
    newCellPlaylistAct->setShortcuts(QKeySequence::New);
    newCellPlaylistAct->setStatusTip(tr("Create a new cell playlist"));
    connect(newCellPlaylistAct, &QAction::triggered, this, &BatlabMainWindow::showNewCellPlaylistWizard);

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

    applicationUpdateController = new QtAutoUpdater::UpdateController("maintenancetool", this, qApp);
    applicationUpdateController->setDetailedUpdateInfo(true);
    checkForUpdatesAct = applicationUpdateController->createUpdateAction(this);
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

void BatlabMainWindow::openCellPlaylist()
{
    // First select the playlist file
    // TODO
    // Then actually load the settings into the GUI
//    loadPlaylistIntoGUI();
}

void BatlabMainWindow::exitBatlabToolkitGUI()
{
    this->close();
}

void BatlabMainWindow::debugBatlab()
{
    mainStackedWidget->setCurrentWidget(logViewTabWidget);
    cellPlaylistButton->setEnabled(true);
    batlabsButton->setEnabled(true);
    logViewButton->setEnabled(false);
    resultsButton->setEnabled(true);
    logViewPrintDebugCheckBox->setChecked(true);
    togglePrintDebugMessages(true);

    if (batlabDebugDialog == nullptr)
    {
        batlabDebugDialog = new BatlabDebugDialog(this, batlabManager->getBatlabInfos(), batlabManager->getFirmwareVersions());
        connect(batlabDebugDialog, &BatlabDebugDialog::registerReadRequested, this, &BatlabMainWindow::processRegisterReadRequest);
        connect(batlabDebugDialog, &BatlabDebugDialog::registerWriteRequested, this, &BatlabMainWindow::processRegisterWriteRequest);
        connect(batlabDebugDialog, &BatlabDebugDialog::firmwareFlashRequested, this, &BatlabMainWindow::processFirmwareFlashRequest);
        connect(batlabManager, &BatlabManager::batlabInfoUpdated, batlabDebugDialog, &BatlabDebugDialog::updateBatlabs);
    }
    else
    {
        batlabDebugDialog->updateInfo(batlabManager->getBatlabInfos(), batlabManager->getFirmwareVersions());
    }

    // Can move window around
    batlabDebugDialog->setModal(false);
    batlabDebugDialog->show();
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

BatlabMainWindow::~BatlabMainWindow()
{

}

void BatlabMainWindow::closeEvent(QCloseEvent *event)
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Batlab Toolkit GUI", "Are you sure you want to quit?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        batlabManager->setAllBatlabChannelsIdle();
        event->accept();
    } else {
        event->ignore();
    }
}

void BatlabMainWindow::updatelogViewWithWriteCommand(int serialNumber, int nameSpace, int batlabRegister, int value)
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

void BatlabMainWindow::updatelogViewWithReadCommand(int serialNumber, int nameSpace, int batlabRegister)
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

void BatlabMainWindow::updatelogViewWithWriteResponse(int nameSpace, int batlabRegister, int lsb, int msb)
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

void BatlabMainWindow::updatelogViewWithReadResponse(int nameSpace, int batlabRegister, int lsb, int msb)
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

void BatlabMainWindow::updatelogViewWithReceivedStream(int cell,int mode,int status,float temp, float current, float voltage)
{
    QString str = QString("STREAM PACKET: ");

//    for (int i = 0; i < BatlabObjects.size(); ++i) {
//        if (sender() == BatlabObjects[i]) {
//            str += QString("Batlab #%1").arg(BatlabObjects[i]->getSerialNumber());
//        }
//    }

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

void BatlabMainWindow::showNewCellPlaylistWizard()
{
    NewCellPlaylistWizard * wizard = new NewCellPlaylistWizard();
    wizard->setWizardStyle(QWizard::ModernStyle);
    wizard->show();
    connect(wizard, &NewCellPlaylistWizard::finished, this, &BatlabMainWindow::loadPlaylistIntoGUI);
}

void BatlabMainWindow::loadPlaylistIntoGUI(CellPlaylist playlist)
{
    mainStackedWidget->setCurrentWidget(cellPlaylistTabWidget);
    cellPlaylistButton->setEnabled(false);
    batlabsButton->setEnabled(true);
    logViewButton->setEnabled(true);
    resultsButton->setEnabled(true);
    cellPlaylistStackedWidget->setCurrentWidget(cellPlaylistLoadedWidget);

    playlistSettingsWidget = new PlaylistSettingsWidget(playlist);

    QVector<cellResultsStatusInfo> cellResultsDisplayInfoVector;

    // ****** Dummy Data *******  // TODO
//    cellResultsStatusInfo newCellResult1 = BatlabLib::createInitializedcellResultsDisplayInfo();
//    newCellResult1.cellName = "CELL_1";
//    newCellResult1.testInProgress = true;
//    newCellResult1.testCompleted = false;
//    cellResultsDisplayInfoVector.push_back(newCellResult1);

//    cellResultsStatusInfo newCellResult2 = BatlabLib::createInitializedcellResultsDisplayInfo();
//    newCellResult2.cellName = "CELL_2";
//    newCellResult2.testInProgress = false;
//    newCellResult2.testCompleted = false;
//    cellResultsDisplayInfoVector.push_back(newCellResult2);

//    cellResultsStatusInfo newCellResult3 = BatlabLib::createInitializedcellResultsDisplayInfo();
//    newCellResult3.cellName = "CELL_3";
//    newCellResult3.testInProgress = false;
//    newCellResult3.testCompleted = true;
//    newCellResult3.chargeCapacity = 9.27;
//    newCellResult3.energyCapacity = 0.23;
//    newCellResult3.avgImpedance = 7.23;
//    newCellResult3.deltaTemperature = 19.23;
//    newCellResult3.avgCurrent = 8.00;
//    newCellResult3.avgVoltage = 4.21;
//    newCellResult3.runtime = 50.22;
//    cellResultsDisplayInfoVector.push_back(newCellResult3);

//    cellResultsStatusInfo newCellResult4 = BatlabLib::createInitializedcellResultsDisplayInfo();
//    newCellResult4.cellName = "CELL_4";
//    newCellResult4.testInProgress = true;
//    newCellResult4.testCompleted = true;
//    newCellResult4.chargeCapacity = 6.43;
//    newCellResult4.energyCapacity = 1.20;
//    newCellResult4.avgImpedance = 9.22;
//    newCellResult4.deltaTemperature = 5.92;
//    newCellResult4.avgCurrent = 339.23;
//    newCellResult4.avgVoltage = 82.40;
//    newCellResult4.runtime = 100.21;
//    cellResultsDisplayInfoVector.push_back(newCellResult4);
    // ****** End Dummy Data *******

    redrawResultsInfo(cellResultsDisplayInfoVector);
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

void BatlabMainWindow::updatelogViewTextBrowser(QString str)
{
    str += "\n";
    str = QString("%1   ").arg(QDateTime::currentDateTime().toString()) + str;
    if (logViewTextBrowser->verticalScrollBar()->value() >= (logViewTextBrowser->verticalScrollBar()->maximum()-10)) {
        logViewTextBrowser->insertPlainText(str);
        logViewTextBrowser->moveCursor(QTextCursor::End);
    } else {
        logViewTextBrowser->insertPlainText(str);
    }
}

void BatlabMainWindow::processQtLogMessage(QtMsgType type, QString str)
{
    QString typeStr = "";
    switch (type) {
    case QtDebugMsg:
        typeStr = "DEBUG";
        break;
    case QtInfoMsg:
        typeStr = "INFO";
        break;
    case QtWarningMsg:
        typeStr = "WARNING";
        break;
    case QtCriticalMsg:
        typeStr = "CRITICAL";
        break;
    case QtFatalMsg:
        typeStr = "FATAL";
        break;
    }
    if (type != QtDebugMsg || printDebugMessages == true)
    {
        updatelogViewTextBrowser(typeStr + ": " + str);
    }
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    BatlabLogger::instance()->handleQtMessage(type, context, msg);
}

void BatlabMainWindow::processRegisterReadRequest(int serial, int ns, int address)
{
    batlabManager->processRegisterReadRequest(serial, ns, address);
}

void BatlabMainWindow::processRegisterWriteRequest(int serial, int ns, int address, int value)
{
    batlabManager->processRegisterWriteRequest(serial, ns, address, value);
}

void BatlabMainWindow::processFirmwareFlashRequest(int serial, QString firmwareVersion)
{
    mainStackedWidget->setCurrentWidget(batlabsTabWidget);
    cellPlaylistButton->setEnabled(true);
    batlabsButton->setEnabled(false);
    logViewButton->setEnabled(true);
    resultsButton->setEnabled(true);
    batlabManager->processFirmwareFlashRequest(serial, firmwareVersion);
}
