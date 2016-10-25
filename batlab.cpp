#include "batlab.h"
#include "ui_batlab.h"
#include <QLineEdit>

Batlab::Batlab(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Batlab)
{
    ui->setupUi(this);
    com = new batlabCom();
    test = new QPushButton(QString("Test"));
    options = new QPushButton(QString("Options"));
    settingsB = new QPushButton(QString("Settings"));
    report = new QPushButton(QString("Report"));
    exit = new QPushButton(QString("Exit"));
    ui->buttonBox->addButton(settingsB,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(test,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(report,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(options,QDialogButtonBox::ActionRole);
    ui->buttonBox->addButton(exit,QDialogButtonBox::ActionRole);
    ui->textBrowser->insertPlainText(QString(">> Batlab analysis results confirm optimal cell matching pairs (1,7), (2,3), (6,8)\n" ));
    ui->textBrowser->insertPlainText(QString(">> Data Analysis confirms Cell 6 as 'Ready for Recycle'\n" ));
    ui->textBrowser->insertPlainText(QString(">> Cell 4 remains unmatched\n" ));
    ui->textBrowser->insertPlainText(QString(">> Exporting analysis data to external report...\n" ));
    ui->textBrowser->insertPlainText(QString(">> Process Complete\n" ));

    connect(com,SIGNAL(emitResponse(int,int,QString,int)),this,SLOT(onReceiveResponse(int,int,QString,int)));
    connect(com,SIGNAL(emitStream(int,int,int,float,int,int,int)),this,SLOT(onReceiveStream(int,int,int,float,int,int,int)));
    connect(com,SIGNAL(emitStreamExt(int,int,int,int,int)),this,SLOT(onReceiveStreamExt(int,int,int,int,int)));
    connect(exit,SIGNAL(clicked()),this,SLOT(close()));
    connect(test,SIGNAL(clicked()),this,SLOT(onTest()));

    batlabDataObject a = batlabDataObject();
    configSettings = new settings();

    connect(this->settingsB,SIGNAL(clicked()),configSettings,SLOT(show()));

QBrush  brush;
QGraphicsScene scene;
QPainter painter;

    //brush.setTextureImage(QImage("1010.bmp"));
    //scene.render(&painter);
    glWidget = ui->graphicsView;

    //painter.drawImage(QRect(0,0,ui->graphicsView->width(),ui->graphicsView->height()),QImage("1010.bmp"));
    //glWidget->drawBackground(&painter,QRect(0,0,ui->graphicsView->width(),ui->graphicsView->height()));
//    glWidget->render(&painter);
    //scene.render(&painter);
   // glWidget->setBackgroundBrush(brush);
    glWidget->show();

}

void Batlab::onTest() {
   if (testObj == nullptr) {
       testObj = new batlabtest();
       connect(testObj,SIGNAL(emitReadReg(int,int,vals)),com,SLOT(onReadReg(int,int,vals)));
       connect(testObj,SIGNAL(emitWriteReg(int,int,writeVals,int)),com,SLOT(onWriteReg(int,int,writeVals,int)));
   }
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
            str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + statusString[aaaa] +"\n";
        }
    } else {
        str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + QString::number(aaaa) +"\n";
    }
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveStream(int unit,int cell,int status,float temp,int current,int voltage,int charge) {
    QString str;
    if (voltage &0x8000) {
        voltage = -0x10000 + voltage;
    }
        str = ">> Unit: " + QString::number(unit) + " Cell: " + QString::number(cell) + " STATUS: " + statusString[status] + " TEMPERATURE: " + QString::number(temp) +" Deg F   CURRENT: " + QString::number(current*5.0/4095) + " A   VOLTAGE: " + QString::number(voltage*5.0/4095) + " V   CHARGE: " + QString::number(charge*5.0/(10.0*1023)) + " C\n";
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}

void Batlab::onReceiveStreamExt(int unit,int cell,int currentAmp,int voltagePhase,int voltageAmp) {
    QString str;
        str = ">> Unit: " + QString::number(unit) + " Cell: " + QString::number(cell) + " Current Ipp: " + QString::number(currentAmp *5.0/1023) + "A-pp  Voltage Phase: " + QString::number(voltagePhase*360/25000) + " Deg  VOLTAGE VPP: " + QString::number(voltageAmp*5.0/4095) + " V-pp\n";
    ui->textBrowser->insertPlainText(str);
    ui->textBrowser->moveCursor(QTextCursor::End);
}
