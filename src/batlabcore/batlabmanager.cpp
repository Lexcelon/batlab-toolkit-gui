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
    // TODO make sure that things are actually Batlabs
    QList<QSerialPortInfo> availableCommPorts = QSerialPortInfo::availablePorts();
    QStringList availableCommPortNames;

    // TODO BUG when Batlabs are removed sometimes they don't show up again when plugged in (on Linux)
    for (int i = 0; i < availableCommPorts.size(); i++) {
        availableCommPortNames.append(availableCommPorts.at(i).portName());
    }

    // Check if Batlabs have been added
    for (int i = 0; i < availableCommPortNames.size(); i++) {
        QString portName = availableCommPortNames[i];
        if (!connectedBatlabsByPortName.contains(portName)) {
            addNewBatlab(portName);
        }
    }

    // Check if Batlabs have been removed
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        if (!availableCommPortNames.contains(portName)) {
            removeBatlab(portName);
        }
    }
}

void BatlabManager::addNewBatlab(QString portName)
{
    Batlab *batlab = new Batlab(portName);
    connect(batlab, &Batlab::infoUpdated, this, &BatlabManager::processUpdatedBatlabInfo);
    connectedBatlabsByPortName[portName] = batlab;
    processUpdatedBatlabInfo();
}

void BatlabManager::removeBatlab(QString portName)
{
    delete connectedBatlabsByPortName[portName];
    connectedBatlabsByPortName.remove(portName);
    processUpdatedBatlabInfo();
}

void BatlabManager::processUpdatedBatlabInfo()
{
    QVector<batlabInfo> infos;
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        batlabInfo info = connectedBatlabsByPortName[portName]->getInfo();
        infos.push_back(info);
    }
    emit batlabInfoUpdated(infos);
}
