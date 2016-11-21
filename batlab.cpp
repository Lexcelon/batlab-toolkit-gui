#include "batlab.h"
#include "ui_batlab.h"


Batlab::Batlab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Batlab)
{
    // Setting up the gui with the same name in the forms directory
    ui->setupUi(this);

    tableWidget = ui->tableWidget;

    // For communication with batlab
    com = new batlabCom();

    // Managing data from cells
    cellManager = new batlabCellManager();

    // GUI for settings
    configSettings = new settings();

    // Create the buttons we use in the main gui
    test = new QPushButton(QString("Test"));
    options = new QPushButton(QString("Options"));
    newProjectWizard = new QPushButton(QString("New Project Wizard"));
    report = new QPushButton(QString("Report"));
    exit = new QPushButton(QString("Exit"));

    // Place the buttons in the button box in our gui
    ui->buttonBox->addButton(newProjectWizard,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(test,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(report,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(options,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(exit,QDialogButtonBox::ActionRole);

    // PLACEHOLDER TEXT WILL REMOVE
    ui->textBrowser->insertPlainText(QString(">> Batlab analysis results confirm optimal cell matching pairs (1,7), (2,3), (6,8)\n" ));
    ui->textBrowser->insertPlainText(QString(">> Data Analysis confirms Cell 6 as 'Ready for Recycle'\n" ));
    ui->textBrowser->insertPlainText(QString(">> Cell 4 remains unmatched\n" ));
    ui->textBrowser->insertPlainText(QString(">> Exporting analysis data to external report...\n" ));
    ui->textBrowser->insertPlainText(QString(">> Process Complete\n" ));

    // Connections for taking data recieved from the comm object and routing it to appropriate functions
    connect(com,SIGNAL(emitResponse(int,int,QString,int)),this,SLOT(onReceiveResponse(int,int,QString,int)));
    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(onReceiveStream(int,int,int,float,int,int,int)));
    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(onReceiveStreamExt(int,int,int,int,int)));

    // Streaming data from comm object to appropriate cell in the cell manager
    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(onReceiveStream(int,int,int,float,int,int,int)));
    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(onReceiveStreamExt(int,int,int,int,int)));


    // Making the buttons functional
    connect(exit,SIGNAL(clicked()),this,SLOT(close()));
    connect(test,SIGNAL(clicked()),this,SLOT(onTest()));
    connect(this->newProjectWizard,SIGNAL(clicked()),this,SLOT(onNewProjectWizard()));

    onAddTests();

}

void Batlab::onTest() {

   // For testing communications with batlab
   if (testObj == nullptr) {
       testObj = new batlabtest();
       connect(testObj,SIGNAL(emitReadReg(int,int,vals)),com,SLOT(onReadReg(int,int,vals)));
       connect(testObj,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
       connect(testObj,SIGNAL(emitPrint(uchar,properties)),cellManager,SLOT(onPrintCell(uchar,properties)));
   }

   //Can move window around
   testObj->setModal(false);
   testObj->show();
}

Batlab::~Batlab()
{
    if (testObj) delete testObj;
    if (com) delete com;
    if (configSettings) delete configSettings;
    if (exit) delete exit;
    if (test) delete test;
    if (options) delete options;
    if (report) delete report;
    if (settingsB) delete settingsB;
    if (newProjectWizard) delete newProjectWizard;
    delete ui;
}

void Batlab::onReceiveResponse(int a,int aa,QString aaa,int aaaa) {
    QString str;
    if (aaa == names[numberOfConnectedUnits]) {
        str = ">> " + aaa + ": " + QString::number((aaaa) & 0x3F);
    } else if (aaa == names[status]){
        if (aaaa == 0x0000) {
            str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + "SUCCEEDED" +"\n";
        } else if (aaaa== 0x0101) {
            str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + "FAILED" +"\n";
        } else {
            str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + parseStatus(aaaa) +"\n";
        }
    } else {
        str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + QString::number(aaaa) +"\n";
    }
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
    QString str;

    // Voltage is a 16 bit signed number but is passed to this function as a 32 bit number
    if (voltage &0x8000) {
        voltage = -0x10000 + voltage;
    }
        str = ">> Unit: " + QString::number(unit) + " Cell: " + QString::number(cell) + " STATUS: " + statusString[status] + " TEMPERATURE: " + QString::number(temp) +" Deg F   CURRENT: " + QString::number(current*1.024*5.0/32768.0) + " A   VOLTAGE: " + QString::number(voltage*4.5/32768.0) + " V   CHARGE: " + QString::number(charge*5.0/(10.0*1023)) + " C\n";
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveStreamExt(int unit,int cell,int currentAmp,int voltagePhase,int voltageAmp) {
    QString str;
        str = ">> Unit: " + QString::number(unit) + " Cell: " + QString::number(cell) + " Current Ipp: " + QString::number(currentAmp *5.0/1023) + "A-pp  Voltage Phase: " + QString::number(voltagePhase*360/25000) + " Deg  VOLTAGE VPP: " + QString::number(voltageAmp*5.0/4095) + " V-pp\n";
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onAddTests() {

    int colCount = 10;
    for (int i = 0; i < colCount; ++i) {
        ui->tableWidget->insertColumn(i);
    }
    for (int i = 0; i < 10; i++) {

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        QTableWidgetItem * item = new QTableWidgetItem("Test1");
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,item);
        for (int j = 1; j < colCount; j++) {
            QTableWidgetItem * testItem = new QTableWidgetItem("CCDC");
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,j,testItem);
        }
//        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    }

    ui->tableWidget->insertRow(4);
}

void Batlab::onNewProjectWizard() {
    batlabWizard a;
    a.onShow();
}
