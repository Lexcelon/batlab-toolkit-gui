#include "batlab.h"

Batlab::Batlab(QString portName, QObject *parent) : QObject(parent)
{
    QState* s_unknown = new QState();
    QState* s_bootloader = new QState();
    QState* s_booted = new QState();
    s_booted->addTransition(this, &Batlab::bootloaderEntered, s_bootloader); // TODO emit this signal when we detect we are in bootloader
    m_batlabStateMachine.addState(s_unknown);
    m_batlabStateMachine.addState(s_bootloader);
    m_batlabStateMachine.addState(s_booted);
    m_batlabStateMachine.setInitialState(s_unknown);
    m_batlabStateMachine.start();

    m_commsManager = new BatlabCommsManager(portName);
    connect(m_commsManager, &BatlabCommsManager::responseBundleReady, this, &Batlab::handleSerialResponseBundleReady);
    connect(m_commsManager, &BatlabCommsManager::packetBundleSendFailed, this, &Batlab::handleSerialPacketBundleSendFailed);

    m_info.externalPowerConnected = false;
    m_info.firmwareVersion = -1;
    m_info.portName = portName;
    m_info.serialNumberRegister = -1;
    m_info.deviceIdRegister = -1;
    m_info.serialNumberComplete = -1;
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

        m_tempCalibB[i] = -1;
        m_tempCalibR[i] = -1;
    }

    verifyBatlabDevice();

    QTimer *batlabPeriodicCheckTimer = new QTimer(this);
    connect(batlabPeriodicCheckTimer, &QTimer::timeout, this, &Batlab::periodicCheck);
    batlabPeriodicCheckTimer->start(5000);
}

void Batlab::handleSerialPacketBundleSendFailed()
{
    // TODO
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
}

void Batlab::verifyBatlabDevice()
{
    QQueue<BatlabPacket> verifyPackets;
    verifyPackets.append(BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::ADDR));
    batlabPacketBundle packetBundle;
    packetBundle.packets = verifyPackets;
    packetBundle.callback = "handleVerifyBatlabDeviceResponse";
    packetBundle.channel = -1;
    m_commsManager->sendPacketBundle(packetBundle);
}

void Batlab::initBatlabDevice()
{
    QQueue<BatlabPacket> initPackets;  // TODO fix namespaces for units and addresses
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

    // TODO check Python code for where we set watchdog timer if firmware > 3
}

void Batlab::handleInitBatlabDeviceResponse(QQueue<BatlabPacket> response)
{
    for (int i = 0; i < 5; i++)
    {
        response.dequeue();
    }
    for (int i = 0; i < 4; i++)
    {
        m_tempCalibR[i] = response.dequeue().value();
    }
    for (int i = 0; i < 4; i++)
    {
        m_tempCalibB[i] = response.dequeue().value();
    }
    for (int i = 0; i < 4; i++)
    {
        response.dequeue();  // Current setpoint (presently unused)
    }
    m_info.serialNumberRegister = response.dequeue().value();
    m_info.deviceIdRegister = response.dequeue().value();
    m_info.serialNumberComplete = (m_info.deviceIdRegister<<16) + m_info.serialNumberRegister;
    m_info.firmwareVersion = response.dequeue().value();

    m_info.externalPowerConnected = response.dequeue().value();

    emit infoUpdated();
}

void Batlab::handleVerifyBatlabDeviceResponse(QQueue<BatlabPacket> response)
{
    BatlabPacket responsePacket = response.dequeue();
    if (responsePacket.value() == 257)
    {
        initBatlabDevice();
    }
    else
    {
        // In bootloader
    }
}

void Batlab::periodicCheck()
{
//    if (info.serialNumberRegister == -1)
//    {
//        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM);
//    }
//    if (info.deviceIdRegister == -1)
//    {
//        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::DEVICE_ID);
//    }
//    if (info.firmwareVersion == -1)
//    {
//        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::FIRMWARE_VER);
//    }
//    for (int i = 0; i < 4; i++)
//    {
//        if (tempCalibB[i] == -1)
//        {
//            initiateRegisterRead(i, cellNamespace::TEMP_CALIB_B);
//        }
//        if (tempCalibR[i] == -1)
//        {
//            initiateRegisterRead(i, cellNamespace::TEMP_CALIB_R);
//        }
//    }

//    initiateRegisterRead(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU);
}

Batlab::~Batlab()
{
}

void Batlab::setAllIdle()
{
    for (int i = 0; i < 4; ++i) {
//        TODO initiateRegisterWrite(i, cellNamespace::MODE, MODE_IDLE);
    }
}

batlabStatusInfo Batlab::getInfo()
{
    return m_info;
}

bool Batlab::hasReceivedValidResponse()
{
    // TODO use state machine to answer this question
    return true;
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

    qDebug() << tr("Batlab connected to port %1 entering bootloader").arg(m_info.portName);

    // Enter bootloader
    QQueue<BatlabPacket> packets;
    BatlabPacket enterBootloaderPacket = BatlabPacket(batlabNamespaces::UNIT, unitNamespace::UNIT_BOOTLOAD, 0x0000);
    enterBootloaderPacket.setSleepAfterTransaction_ms(2000);
    packets.append(enterBootloaderPacket);

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
        BatlabPacket firmwareAddrPacket = BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::ADDR, 0x0400 + i);
        packets.append(firmwareAddrPacket);
        BatlabPacket firmwareDataPacket = BatlabPacket(batlabNamespaces::BOOTLOADER, bootloaderNamespace::DATA, firmwareBytes[i]);
        packets.append(firmwareDataPacket);
    }

    // Reboot into new image
    // self.write(BOOTLOADER,BL_BOOTLOAD,0x0000)
    // sleep(2)
    // if(self.read(BOOTLOADER,BL_DATA).value() == COMMAND_ERROR):
        // self.bootloader = False
        // self.sn = int(self.read(UNIT,SERIAL_NUM).value()) + (int(self.read(UNIT,DEVICE_ID).value()) << 16)
        // print("Connected to Batlab " + str(self.sn))
        // fw = int(self.read(UNIT,FIRMWARE_VER).value())
        // print("Firmware Version " + str(fw))
        // return True
    // else:
        // print("Batlab still in Bootloader -- Try again")
        // return False

    batlabPacketBundle bundle;
    bundle.packets = packets;
    bundle.callback = "handleFirmwareFlashResponse";
    bundle.channel = -1;
    m_commsManager->sendPacketBundle(bundle);
}
