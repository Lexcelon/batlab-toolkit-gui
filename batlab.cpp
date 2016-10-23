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
   if (testObj == nullptr) testObj = new batlabtest();
   connect(testObj,SIGNAL(emitGetCharge(int,int)),com,SLOT(onGetCharge(int,int)));
   connect(testObj,SIGNAL(emitGetTemp(int,int)),com,SLOT(onGetTemp(int,int)));
   connect(testObj,SIGNAL(emitGetEnergy(int,int)),com,SLOT(onGetEnergy(int,int)));
   connect(testObj,SIGNAL(emitGetVol(int,int)),com,SLOT(onGetVol(int,int)));
   connect(testObj,SIGNAL(emitGetVolAmp(int,int)),com,SLOT(onGetVolAmp(int,int)));
   connect(testObj,SIGNAL(emitGetVolPhase(int,int)),com,SLOT(onGetVolPhase(int,int)));
   connect(testObj,SIGNAL(emitGetCurr(int,int)),com,SLOT(onGetCurr(int,int)));
   connect(testObj,SIGNAL(emitGetCurrAmp(int,int)),com,SLOT(onGetCurrAmp(int,int)));
   testObj->setModal(false);
   testObj->show();

}

Batlab::~Batlab()
{
    if (configSettings) delete configSettings;
    if (exit) delete exit;
    if (test) delete test;
    if (options) delete options;
    if (report) delete report;
    if (settingsB) delete settingsB;
    delete ui;
}

void Batlab::onReceiveResponse(int a,int aa,QString aaa,int aaaa) {
    qDebug() << a << aa << aaa << aaaa;
    QString str = ">> Unit: " + QString::number(a) + " Cell: " + QString::number(aa) + " " + aaa + ": " + QString::number(aaaa) +"\n";
    ui->textBrowser->insertPlainText(str);
}
