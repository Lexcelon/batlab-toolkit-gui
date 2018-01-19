#include "batlabwidget.h"

BatlabWidget::BatlabWidget(batlabInfo info, QWidget *parent) : QWidget(parent)
{
    batlabWidgetLayout = new QGridLayout;

    serialNumberLabel = new QLabel(tr("Batlab Serial Number:"));
    serialNumberValueLabel = new QLabel(QString::number(info.serialNumber));

    portNameLabel = new QLabel(tr("Serial Port Name:"));
    portNameValueLabel = new QLabel(info.portName);

    externalPowerConnectedLabel = new QLabel(tr("External Power Connected:"));
    externalPowerConnectedValueLabel = new QLabel(info.externalPowerConnected ? tr("Yes") : tr("No"));

    firmwareVersionLabel = new QLabel(tr("Firmware Version:"));
    firmwareVersionValueLabel = new QLabel(QString::number(info.firmwareVersion));

    batlabWidgetLayout->addWidget(serialNumberLabel, 0, 0);
    batlabWidgetLayout->addWidget(serialNumberValueLabel, 0, 1);
    batlabWidgetLayout->addWidget(portNameLabel, 1, 0);
    batlabWidgetLayout->addWidget(portNameValueLabel, 1, 1);
    batlabWidgetLayout->addWidget(externalPowerConnectedLabel, 2, 0);
    batlabWidgetLayout->addWidget(externalPowerConnectedValueLabel, 2, 1);
    batlabWidgetLayout->addWidget(firmwareVersionLabel, 3, 0);
    batlabWidgetLayout->addWidget(firmwareVersionValueLabel, 3, 1);

    this->setLayout(batlabWidgetLayout);
    this->setAutoFillBackground(true);
}
