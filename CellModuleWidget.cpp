#include "CellModuleWidget.h"
#include "ui_CellModuleWidget.h"
#include <QGroupBox>
#include <QDebug>
#include <QString>
#include <QFont>

CellModuleWidget::CellModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellModuleWidget)
{
    ui->setupUi(this);
}

CellModuleWidget::CellModuleWidget(int moduleNumber, QStringList cellList, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellModuleWidget)
{
    ui->setupUi(this);
    QString string = QString("Module %1").arg(moduleNumber);
    (ui->moduleBox)->setTitle(string);
    (ui->moduleBox)->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;}"
                            " QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;}");
    for (int i = 0; i < cellList.size(); ++i) {
        QGroupBox *box = new QGroupBox(cellList[i]);
        box->setStyleSheet("QGroupBox{border:2px solid gray;border-radius:5px;margin-top: 1ex;}"
                                " QGroupBox::title{subcontrol-origin: padding;subcontrol-position:top center;padding:0 0px;}");
        box->setMaximumHeight(100);
        QFont *font = const_cast<QFont*>(&(box->font()));
        font->setBold(true);
        font->setPointSize(10);
        box->setFont(*font);
        ui->horizontalLayout->addWidget(box);
    }
}

CellModuleWidget::~CellModuleWidget()
{
    delete ui;
}
