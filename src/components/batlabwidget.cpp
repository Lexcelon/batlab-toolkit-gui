#include "batlabwidget.h"


BatlabWidget::BatlabWidget(batlabStatusInfo info, int latestFirmwareVersion, QFrame *parent) : QFrame(parent)
{
    m_info = info;
    m_latestFirmwareVersion = latestFirmwareVersion;

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
    connect(firmwareVersionUpdateButton, &QAbstractButton::released, this, &BatlabWidget::updateFirmware, Qt::QueuedConnection);

    batlabInfoLayout->addWidget(serialNumberLabel, 0, 0);
    batlabInfoLayout->addWidget(serialNumberValueLabel, 0, 1);
    batlabInfoLayout->addWidget(portNameLabel, 1, 0);
    batlabInfoLayout->addWidget(portNameValueLabel, 1, 1);
    batlabInfoLayout->addWidget(externalPowerConnectedLabel, 2, 0);
    batlabInfoLayout->addWidget(externalPowerConnectedValueLabel, 2, 1);
    batlabInfoLayout->addWidget(firmwareVersionLabel, 3, 0);
    batlabInfoLayout->addWidget(firmwareVersionValueLabel, 3, 1);

    if (info.firmwareVersion != -1 && info.firmwareVersion < latestFirmwareVersion && info.firmwareBytesRemaining == -1) {
        batlabInfoLayout->addWidget(firmwareVersionUpdateButton, 4, 0);
    }

    batlabInfoWidget->setLayout(batlabInfoLayout);
    batlabInfoWidget->setFixedWidth(275);

    // SETUP LINE
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);

    batlabLayout->addWidget(batlabInfoWidget);
    batlabLayout->addWidget(line);

    if (info.firmwareBytesRemaining == -1)
    {

        // SETUP CELL INFO WIDGET
        QWidget *batlabCellInfoWidget = new QWidget;
        batlabCellInfoLayout = new QGridLayout;

        // TODO if channel does not have anything going on, make it prettier
        for (int i = 0; i < 4; i++) {
            CellTestStatusWidgetList[i] = new CellTestStatusWidget(info.channels[i]);
            QLabel *cellNameLabel = new QLabel(tr(info.channels[i].cellName.toStdString().c_str()));
            cellNameLabel->setFixedWidth(50);

            // Add Batlab Cell Widgets to View Layout
            batlabCellInfoLayout->addWidget(cellNameLabel, i, 0);
            batlabCellInfoLayout->addWidget(CellTestStatusWidgetList[i], i, 1);
        }

        batlabCellInfoWidget->setLayout(batlabCellInfoLayout);
        batlabLayout->addWidget(batlabCellInfoWidget);
    }
    else
    {
        QWidget *firmwareProgress = new QWidget;
        QVBoxLayout *firmwareProgressLayout = new QVBoxLayout;
        firmwareProgressLayout->addStretch();
        QLabel *firmwareProgressText = new QLabel(tr("Firmware update in progress. Please do not disconnect Batlab."));
        firmwareProgressText->setAlignment(Qt::AlignCenter);
        firmwareProgressLayout->addWidget(firmwareProgressText);
        QProgressBar *firmwareProgressBar = new QProgressBar;
        firmwareProgressBar->setMinimum(0);
        firmwareProgressBar->setMaximum(FIRMWARE_FILE_SIZE * 2);
        firmwareProgressBar->setValue(firmwareProgressBar->maximum() - info.firmwareBytesRemaining);
        firmwareProgressLayout->addWidget(firmwareProgressBar);
        firmwareProgressLayout->addStretch();
        firmwareProgress->setLayout(firmwareProgressLayout);
        batlabLayout->addWidget(firmwareProgress);
    }

    this->setLayout(batlabLayout);
    this->setAutoFillBackground(true);

    // Fix the Height and Create a Border
    this->setFixedHeight(200);
    this->setStyleSheet("QFrame BatlabWidget{ border: 2px solid gray; border-radius:5px; }");
}

void BatlabWidget::updateFirmware() {
    // TODO This message box is causing seg faults, going to just disable and can figure it out later if we want the box
//    QMessageBox messageBox(this);
//    messageBox.setWindowTitle(tr("Firmware Update"));
//    messageBox.setText(tr("The firmware on Batlab %1 will be updated from v%2.0 to v%3.0.\n\n"
//                          "Please do not disconnect your Batlab during the update."
//                          "This process can take a few minutes.")
//                       .arg(m_info.serialNumberComplete)
//                       .arg(m_info.firmwareVersion)
//                       .arg(m_latestFirmwareVersion));
//    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//    messageBox.setDefaultButton(QMessageBox::Ok);
//    int ret = messageBox.exec();
//    if (ret == QMessageBox::Ok)
//    {
        emit firmwareUpdateRequested(m_info.serialNumberComplete, "v" + QString::number(m_latestFirmwareVersion) + ".0");
//    }
}
