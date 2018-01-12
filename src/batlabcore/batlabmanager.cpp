#include "batlabmanager.h"

BatlabManager::BatlabManager(QObject *parent) : QObject(parent)
{
    cellPlaylistLoaded = false;
    testsInProgress = false;

    QTimer *updateConnectedBatlabsTimer = new QTimer(this);
    connect(updateConnectedBatlabsTimer, &QTimer::timeout, this, &BatlabManager::updateConnectedBatlabs);
    updateConnectedBatlabsTimer->start(1000);
}

void BatlabManager::updateConnectedBatlabs()
{
    QList<QSerialPortInfo> availCommPorts = QSerialPortInfo::availablePorts();
    QStringList availCommPortNames;

    for (int i = 0; i < availCommPorts.size(); i++) {
        availCommPortNames.append(availCommPorts.at(i).portName());
    }

    qWarning() << QSerialPortInfo::availablePorts().size();
}
