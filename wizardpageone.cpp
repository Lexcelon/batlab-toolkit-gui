#include "wizardpageone.h"
#include "ui_wizardpageone.h"

wizardPageOne::wizardPageOne(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wizardPageOne)
{
    ui->setupUi(this);

    ui->startingIndex->setValidator(new QIntValidator(0,0xFFFF));


    connect(ui->name,SIGNAL(editingFinished()),this,SLOT(onDesignator()));
    connect(ui->numCells,SIGNAL(valueChanged(int)),this,SLOT(onNumCells(int)));
    connect(ui->startingIndex,SIGNAL(editingFinished()),this,SLOT(onStartInd()));
    connect(ui->projectName, SIGNAL(editingFinished()), this,SLOT(onProjectName()));
}

void wizardPageOne::onFinish() {
    QString txt = ui->startingIndex->text();
    int zeros = txt.length() - QString::number(txt.toInt()).length()+1;
    txt = QString::number(txt.toInt()+1);
    ui->startingIndex->setText(QString("%1").arg(txt.toInt(),zeros,10,QChar('0')));
}

void wizardPageOne::onUpdate() {
    emit emitDesignator(ui->name->text());
    emit emitStartInd(ui->startingIndex->text());
    emit emitNumCells(ui->numCells->value());
    emit emitProjectName(ui->projectName->text());
}

wizardPageOne::~wizardPageOne()
{
    delete ui;
}


void wizardPageOne::onDesignator() {
    name = ui->name->text();
    emit emitDesignator(name);
}

void wizardPageOne::onStartInd() {
    startInd = ui->startingIndex->text();
    emit emitStartInd(startInd);
}

void wizardPageOne::onNumCells(int val) {
    numCells = val;
    emit emitNumCells(val);
}

void wizardPageOne::onNew(bool val) {
    isNew = val;
    emit emitIsNew(val);
}

void wizardPageOne::onProjectName() {
    projectName = ui->projectName->text();
    emit emitProjectName(projectName);
}
