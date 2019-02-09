#include "batlab.h"

Batlab::Batlab(QString portName, QObject *parent) : QObject(parent)
{
    m_commsManager = new BatlabCommsManager(portName);
    connect(m_commsManager, &BatlabCommsManager::responseBundleReady, this, &Batlab::handleSerialResponseBundleReady);
    connect(m_commsManager, &BatlabCommsManager::packetBundleSendFailed, this, &Batlab::handleSerialPacketBundleSendFailed);
    connect(m_commsManager, &BatlabCommsManager::firmwareFlashProgress, this, &Batlab::updateFirmwareFlashProgress);

    m_info.confirmedBatlabDevice = false;
    m_info.inBootloader = false;
    m_info.externalPowerConnected = false;
    m_info.firmwareVersion = -1;
    m_info.portName = portName;
    m_info.serialNumberRegister = -1;
    m_info.deviceIdRegister = -1;
    m_info.serialNumberComplete = -1;
    m_info.firmwareBytesRemaining = -1;
    for (int i = 0; i < 4; i++)
    {
        m_info.channels[i].cellName = "";
        m_info.channels[i].testInProgress = false;
        m_info.channels[i].preChargeComplete = false;
        m_info.channels[i].preChargeError = false;
        m_info.channels[i].numWarmupCycles = -1;
        m_info.channels[i].numWarmupCyclesCompleted = -1;
        m_info.channels[i].warmupCyclesError = false;
        m_info.channels[i].numMeasurementCycles = -1;
        m_info.channels[i].numMeasurementCyclesCompleted = -1;
        m_info.channels[i].measurementCyclesError = false;
        m_info.channels[i].storageDischarge = false;
        m_info.channels[i].storageDischargeComplete = false;
        m_info.channels[i].storageDischargeError = false;
        m_info.channels[i].tempCalibB = -1;
        m_info.channels[i].tempCalibR = -1;
    }

    verifyBatlabDevice();

    QTimer *batlabPeriodicCheckTimer = new QTimer(this);
    connect(batlabPeriodicCheckTimer, &QTimer::timeout, this, &Batlab::periodicCheck);
    batlabPeriodicCheckTimer->start(5000);
}

void Batlab::updateFirmwareFlashProgress(int packetsRemaining)
{
    m_info.firmwareBytesRemaining = packetsRemaining;
    emit infoUpdated();
}

void Batlab::handleSerialPacketBundleSendFailed(batlabPacketBundle bundle)
{
    if (bundle.callback == "handleVerifyBatlabDeviceResponse")
    {
        qWarning() << tr("Failed to verify Batlab device on port %1").arg(m_info.portName);
    }
    else if (bundle.callback == "handleInitBatlabDeviceResponse")
    {
        qWarning() << tr("Serial failure during Batlab initialization on port %1").arg(m_info.portName);
    }
    else if (bundle.callback == "handlePeriodicCheckResponse")
    {
        qWarning() << tr("Serial failure during periodic check on port %1").arg(m_info.portName);
    }
    else if (bundle.callback == "handleFirmwareFlashResponse")
    {
        qWarning() << tr("Unable to flash firmware on device on port %1").arg(m_info.portName);
    }
    else
    {
        qWarning() << tr("Error during serial communication on port %1").arg(m_info.portName);
    }
}

void Batlab::handleSerialResponseBundleReady(batlabPacketBundle bundle)
{
    if (bundle.callback == "handleVerifyBatlabDeviceResponse")
    {
        handleVerifyBatlabDeviceResponse(bundle.packets);
    }
    else if (bundle.callback == "handleInitBatlabDeviceResponse")
    {
        handleInitBatlabDeviceResponse(bundle.packets);
    }
    else if (bundle.callback == "handlePeriodicCheckResponse")
    {
        handlePeriodicCheckResponse(bundle.packets);
    }
    else if (bundle.callback == "setWatchdogTimerResponse")
    {
        handleSetWatchdogTimerResponse(bundle.packets);
    }
    else if (bundle.callback == "handleFirmwareFlashResponse")
    {
        handleFirmwareFlashResponse(bundle.packets);
    }
    else if (bundle.callback == "handleRegisterReadResponse")
    {
        handleRegisterReadResponse(bundle.packets);
    }
    else if (bundle.callback == "handleRegisterWriteResponse")
    {
        handleRegisterWriteResponse(bundle.packets);
    }
}

void Batlab::verifyBatlabDevice()
{
    QVector<BatlabPacket> verifyPackets;
    verifyPackets.append(BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::ADDR));
    batlabPacketBundle packetBundle;
    packetBundle.packets = verifyPackets;
    packetBundle.callback = "handleVerifyBatlabDeviceResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::registerRead(int ns, int address)
{
    QVector<BatlabPacket> packets;
    packets.append(BatlabPacket(ns, address));
    batlabPacketBundle packetBundle;
    packetBundle.packets = packets;
    packetBundle.callback = "handleRegisterReadResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::registerWrite(int ns, int address, int value)
{
    QVector<BatlabPacket> packets;
    packets.append(BatlabPacket(ns, address, value));
    batlabPacketBundle packetBundle;
    packetBundle.packets = packets;
    packetBundle.callback = "handleRegisterWriteResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::initBatlabDevice()
{
    QVector<BatlabPacket> initPackets;
    initPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::WATCHDOG_TIMER, WATCHDOG_TIMER_RESET));

    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::MODE, MODE_IDLE));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::MODE, MODE_IDLE));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::MODE, MODE_IDLE));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::MODE, MODE_IDLE));

    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_R));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_R));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_R));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_R));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_B));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_B));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_B));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_B));

    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::CURRENT_SETPOINT));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::CURRENT_SETPOINT));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::CURRENT_SETPOINT));
    initPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::CURRENT_SETPOINT));

    initPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM));
    initPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::DEVICE_ID));
    initPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::FIRMWARE_VER));

    initPackets.append(BatlabPacket(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU));

    batlabPacketBundle packetBundle;
    packetBundle.packets = initPackets;
    packetBundle.callback = "handleInitBatlabDeviceResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::handleInitBatlabDeviceResponse(QVector<BatlabPacket> response)
{
    int responseCounter = 0;
    responseCounter += 5;  // Skip cell mode return values
    for (int i = 0; i < 4; i++)
    {
        m_info.channels[i].tempCalibR = response[responseCounter++].value();
    }
    for (int i = 0; i < 4; i++)
    {
        m_info.channels[i].tempCalibB = response[responseCounter++].value();
    }
    responseCounter += 4;  // Skip current setpoint (presently unused)
    m_info.serialNumberRegister = response[responseCounter++].value();
    m_info.deviceIdRegister = response[responseCounter++].value();
    m_info.serialNumberComplete = (m_info.deviceIdRegister<<16) + m_info.serialNumberRegister;
    m_info.firmwareVersion = response[responseCounter++].value();

    if (m_info.firmwareVersion > 3)
    {
        setWatchdogTimer();
    }

    m_info.externalPowerConnected = response[responseCounter++].value();

    emit infoUpdated();
}

void Batlab::setWatchdogTimer()
{
    QVector<BatlabPacket> packets;
    packets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SETTINGS, SET_WATCHDOG_TIMER));
    batlabPacketBundle packetBundle;
    packetBundle.packets = packets;
    packetBundle.callback = "handleSetWatchdogTimerResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::handleSetWatchdogTimerResponse(QVector<BatlabPacket> response)
{
}

void Batlab::handleVerifyBatlabDeviceResponse(QVector<BatlabPacket> response)
{
    m_info.confirmedBatlabDevice = true;
    BatlabPacket responsePacket = response[0];
    if (responsePacket.value() == 257)
    {
       initBatlabDevice();
    }
    else
    {
        m_info.inBootloader = true;
    }
}

void Batlab::handleRegisterReadResponse(QVector<BatlabPacket> response)
{
    BatlabPacket responsePacket = response[0];
    responsePacket.debug();
}

void Batlab::handleRegisterWriteResponse(QVector<BatlabPacket> response)
{
    BatlabPacket responsePacket = response[0];
    responsePacket.debug();
}

void Batlab::periodicCheck()
{
    if (!m_info.confirmedBatlabDevice || m_info.inBootloader) { return; }

    QVector<BatlabPacket> checkPackets;
    checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::WATCHDOG_TIMER, WATCHDOG_TIMER_RESET));

    checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM));
    checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::DEVICE_ID));
    checkPackets.append(BatlabPacket(batlabNamespaces::UNIT, unitNamespace::FIRMWARE_VER));

    checkPackets.append(BatlabPacket(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU));

    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_R));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_R));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_R));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_R));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_B));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_B));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_B));
    checkPackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_B));

    batlabPacketBundle packetBundle;
    packetBundle.packets = checkPackets;
    packetBundle.callback = "handlePeriodicCheckResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::handlePeriodicCheckResponse(QVector<BatlabPacket> response)
{
    int responseCounter = 0;
    responseCounter += 1;  // Skip watchdog return value

    m_info.serialNumberRegister = response[responseCounter++].value();
    m_info.deviceIdRegister = response[responseCounter++].value();
    m_info.serialNumberComplete = (m_info.deviceIdRegister<<16) + m_info.serialNumberRegister;
    m_info.firmwareVersion = response[responseCounter++].value();

    m_info.externalPowerConnected = response[responseCounter++].value();

    for (int i = 0; i < 4; i++)
    {
        m_info.channels[i].tempCalibR = response[responseCounter++].value();
    }
    for (int i = 0; i < 4; i++)
    {
        m_info.channels[i].tempCalibB = response[responseCounter++].value();
    }

    emit infoUpdated();
}

Batlab::~Batlab()
{
    delete m_commsManager;
}

void Batlab::setAllIdle()  // TODO validate this behavior
{
    QVector<BatlabPacket> idlePackets;

    idlePackets.append(BatlabPacket(batlabNamespaces::CHANNEL0, cellNamespace::MODE, MODE_IDLE));
    idlePackets.append(BatlabPacket(batlabNamespaces::CHANNEL1, cellNamespace::MODE, MODE_IDLE));
    idlePackets.append(BatlabPacket(batlabNamespaces::CHANNEL2, cellNamespace::MODE, MODE_IDLE));
    idlePackets.append(BatlabPacket(batlabNamespaces::CHANNEL3, cellNamespace::MODE, MODE_IDLE));

    batlabPacketBundle packetBundle;
    packetBundle.packets = idlePackets;
    packetBundle.callback = "handleSetAllIdleResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

batlabStatusInfo Batlab::getInfo()
{
    return m_info;
}

bool Batlab::hasReceivedValidResponse()
{
    return m_info.confirmedBatlabDevice;
}

void Batlab::initiateFirmwareFlash(QString firmwareFilePath)
{
    // Open firmware file and verify size
    QFileInfo firmwareFileInfo(firmwareFilePath);
    if (firmwareFileInfo.size() != FIRMWARE_FILE_SIZE)
    {
        qWarning() << "Attempted to flash firmware file of invalid size";
        return;
    }

    qInfo() << tr("Batlab connected to port %1 entering bootloader").arg(m_info.portName);

    // Enter bootloader
    QVector<BatlabPacket> packets;

    if (!m_info.inBootloader)
    {
        BatlabPacket enterBootloaderPacket = BatlabPacket(batlabNamespaces::UNIT, unitNamespace::UNIT_BOOTLOAD, 0x0000);
        enterBootloaderPacket.setSleepAfterTransaction_ms(2000);
        packets.append(enterBootloaderPacket);
    }

    QFile firmwareFile(firmwareFilePath);
    if (!firmwareFile.open(QIODevice::ReadOnly))
    {
        qWarning() << tr("Unable to open firmware file");
        return;
    }
    QByteArray firmwareBytes = firmwareFile.readAll();

    // Write firmware payload
    for(int i = 0; i < firmwareBytes.size(); i++)
    {
        packets.append(BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::ADDR, 0x0400 + i));
        packets.append(BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::DATA, firmwareBytes[i]));
    }

    // Reboot into new image
    BatlabPacket rebootPacket = BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::BOOTLOAD, 0x0000);
    rebootPacket.setSleepAfterTransaction_ms(2000);
    packets.append(rebootPacket);
    packets.append(BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::DATA));

    batlabPacketBundle bundle;
    bundle.packets = packets;
    bundle.callback = "handleFirmwareFlashResponse";
    bundle.channel = -1;
    m_commsManager->sendPacketBundle(bundle);
}

void Batlab::handleFirmwareFlashResponse(QVector<BatlabPacket> response)
{
    qInfo() << "Done with firmware flash.";
    m_info.firmwareBytesRemaining = -1;
    m_info.inBootloader = false;
    emit infoUpdated();
}
