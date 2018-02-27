#include "batlabwidget.h"

BatlabWidget::BatlabWidget(batlabInfo info, QFrame *parent) : QFrame(parent)
{
    // **Chose to use QFrame instead of QWidget due to outline paint limitations
    batlabLayout = new QHBoxLayout;

    // SETUP BATLAB INFO WIDGET
    QWidget *batlabInfoWidget = new QWidget;

    batlabInfoLayout = new QGridLayout;

    serialNumberLabel = new QLabel(tr("Batlab Serial #:"));
    serialNumberValueLabel = new QLabel(info.serialNumberComplete == -1 ? "" : QString::number(info.serialNumberComplete));

    portNameLabel = new QLabel(tr("Port Name:"));
    portNameValueLabel = new QLabel(info.portName);

    externalPowerConnectedLabel = new QLabel(tr("External Power:"));
    externalPowerConnectedValueLabel = new QLabel(info.externalPowerConnected ? tr("Yes") : tr("No"));

    firmwareVersionLabel = new QLabel(tr("Firmware Version:"));
    firmwareVersionValueLabel = new QLabel(info.firmwareVersion == -1 ? "" : QString::number(info.firmwareVersion));
    firmwareVersionUpdateButton = new QPushButton(tr("Update Firmware"));
    connect(firmwareVersionUpdateButton, &QAbstractButton::released, this, &BatlabWidget::updateFirmware);

    // TODO: Update the current firmware version
    if (info.firmwareVersion == 4.0) {
        firmwareVersionUpdateButton->setEnabled(false);
    }

    batlabInfoLayout->addWidget(serialNumberLabel, 0, 0);
    batlabInfoLayout->addWidget(serialNumberValueLabel, 0, 1);
    batlabInfoLayout->addWidget(portNameLabel, 1, 0);
    batlabInfoLayout->addWidget(portNameValueLabel, 1, 1);
    batlabInfoLayout->addWidget(externalPowerConnectedLabel, 2, 0);
    batlabInfoLayout->addWidget(externalPowerConnectedValueLabel, 2, 1);
    batlabInfoLayout->addWidget(firmwareVersionLabel, 3, 0);
    batlabInfoLayout->addWidget(firmwareVersionValueLabel, 3, 1);
    batlabInfoLayout->addWidget(firmwareVersionUpdateButton, 4, 1);

    batlabInfoWidget->setLayout(batlabInfoLayout);
    batlabInfoWidget->setFixedWidth(275);

    // SETUP LINE
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);

    // SETUP CELL INFO WIDGET
    QWidget *batlabCellInfoWidget = new QWidget;
    batlabCellInfoLayout = new QGridLayout;

    for (int i = 0; i < 4; i++) {
        CellTestStatusWidgetList[i] = new CellTestStatusWidget(info.channels[i]);
        QLabel *cellNameLabel = new QLabel(tr(info.channels[i].cellName.toStdString().c_str()));
        cellNameLabel->setFixedWidth(50);

        // Add Batlab Cell Widgets to View Layout
        batlabCellInfoLayout->addWidget(cellNameLabel, i, 0);
        batlabCellInfoLayout->addWidget(CellTestStatusWidgetList[i], i, 1);
    }

    batlabCellInfoWidget->setLayout(batlabCellInfoLayout);

    // Add All to Batlab Widget
    batlabLayout->addWidget(batlabInfoWidget);
    batlabLayout->addWidget(line);
    batlabLayout->addWidget(batlabCellInfoWidget);

    this->setLayout(batlabLayout);
    this->setAutoFillBackground(true);

    // Fix the Height and Create a Border
    this->setFixedHeight(200);
    this->setStyleSheet("QFrame BatlabWidget{ border: 2px solid gray; border-radius:5px; }");
}

void BatlabWidget::updateFirmware() {
    // TODO: Update the Firmware
}
