#include "batlabmanager.h"

BatlabManager::BatlabManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QVector<uchar>>("QVector<uchar>");

    cellPlaylistLoaded = false;
    testsInProgress = false;

    QTimer *updateConnectedBatlabsTimer = new QTimer(this);
    connect(updateConnectedBatlabsTimer, &QTimer::timeout, this, &BatlabManager::updateConnectedBatlabs);
    updateConnectedBatlabsTimer->start(200);
}

void BatlabManager::updateConnectedBatlabs()
{
    // TODO make sure that things are actually Batlabs
    QList<QSerialPortInfo> availableCommPorts = QSerialPortInfo::availablePorts();
    QStringList availableCommPortNames;

    for (int i = 0; i < availableCommPorts.size(); i++) {
        // On Windows availableports() returns those that are not active, meaning you have to check individually
        // to see if they are active. Otherwise it never thinks you unplug a Batlab.
        // https://stackoverflow.com/questions/24854597/qserialportinfo-returns-more-com-ports-than-i-have
        // https://bugreports.qt.io/browse/QTBUG-39748
        if (!availableCommPorts.at(i).description().isEmpty()
                && availableCommPorts.at(i).vendorIdentifier() == 0x04D8
                && availableCommPorts.at(i).productIdentifier() == 0x000A)
        {
            availableCommPortNames.append(availableCommPorts.at(i).portName());
        }
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
    QVector<batlabDisplayInfo> infos;
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        // Only show information for devices that have received valid responses (i.e. are Batlabs)
        if (connectedBatlabsByPortName[portName]->hasReceivedValidResponse())
        {
            batlabDisplayInfo info = connectedBatlabsByPortName[portName]->getInfo();
            infos.push_back(info);
        }
    }
    emit batlabInfoUpdated(infos);
}

QVector<batlabDisplayInfo> BatlabManager::getBatlabInfos()
{
    QVector<batlabDisplayInfo> infos;
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        // Only return information for devices that have received valid responses (i.e. are Batlabs)
        if (connectedBatlabsByPortName[portName]->hasReceivedValidResponse())
        {
            batlabDisplayInfo info = connectedBatlabsByPortName[portName]->getInfo();
            infos.push_back(info);
        }
    }
    return infos;
}

void BatlabManager::processRegisterReadRequest(int serial, int ns, int address)
{
    for (int i = 0; i < connectedBatlabsByPortName.size(); i++)
    {
        QString portName = connectedBatlabsByPortName.keys()[i];
        batlabDisplayInfo info = connectedBatlabsByPortName[portName]->getInfo();
        if (info.serialNumberComplete == serial)
        {
            connectedBatlabsByPortName[portName]->initiateRegisterRead(ns, address);
        }
    }
}

void BatlabManager::processRegisterWriteRequest(int serial, int ns, int address, int value)
{
    for (int i = 0; i < connectedBatlabsByPortName.size(); i++)
    {
        QString portName = connectedBatlabsByPortName.keys()[i];
        batlabDisplayInfo info = connectedBatlabsByPortName[portName]->getInfo();
        if (info.serialNumberComplete == serial)
        {
            connectedBatlabsByPortName[portName]->initiateRegisterWrite(ns, address, value);
        }
    }
}
