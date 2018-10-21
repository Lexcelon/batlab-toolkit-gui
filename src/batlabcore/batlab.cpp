#include "batlab.h"

Batlab::Batlab(QString portName, QObject *parent) : QObject(parent)
{
    QState* s_unknown = new QState();
    QState* s_bootloader = new QState();
    QState* s_booted = new QState();
    s_booted->addTransition(this, &Batlab::bootloaderEntered, s_bootloader); // TODO emit this signal when we detect we are in bootloader
    batlabStateMachine.addState(s_unknown);
    batlabStateMachine.addState(s_bootloader);
    batlabStateMachine.addState(s_booted);
    batlabStateMachine.setInitialState(s_unknown);
    batlabStateMachine.start();

    m_commsManager = new BatlabCommsManager(portName);
    connect(m_commsManager, &BatlabCommsManager::responseBundleReady, this, &Batlab::handleSerialResponseBundleReady);
    connect(m_commsManager, &BatlabCommsManager::packetBundleSendFailed, this, &Batlab::handleSerialPacketBundleSendFailed);

    info.externalPowerConnected = false;
    info.firmwareVersion = -1;
    info.portName = portName;
    info.serialNumberRegister = -1;
    info.deviceIdRegister = -1;
    info.serialNumberComplete = -1;
    for (int i = 0; i < 4; i++)
    {
        info.channels[i].cellName = "";
        info.channels[i].testInProgress = false;
        info.channels[i].preChargeComplete = false;
        info.channels[i].preChargeError = false;
        info.channels[i].numWarmupCycles = -1;
        info.channels[i].numWarmupCyclesCompleted = -1;
        info.channels[i].warmupCyclesError = false;
        info.channels[i].numMeasurementCycles = -1;
        info.channels[i].numMeasurementCyclesCompleted = -1;
        info.channels[i].measurementCyclesError = false;
        info.channels[i].storageDischarge = false;
        info.channels[i].storageDischargeComplete = false;
        info.channels[i].storageDischargeError = false;

        tempCalibB[i] = -1;
        tempCalibR[i] = -1;
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

void Batlab::initiateRegisterRead(int batlabNamespace, int batlabRegister)
{
    emit registerReadInitiated(info.serialNumberRegister, batlabNamespace, batlabRegister);

    QVector<uchar> data(5);
    data[0] = static_cast<uchar>(0xAA);
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister);
    data[3] = static_cast<uchar>(0x00);
    data[4] = static_cast<uchar>(0x00);

//    this->serialTransaction(1000, data);
}


void Batlab::initiateRegisterWrite(int batlabNamespace, int batlabRegister, int num)
{
    emit registerWriteInitiated(info.serialNumberRegister, batlabNamespace, batlabRegister, num);

    QVector<uchar> data(5);

    uchar msb = ((uchar)((0xFF00 & num) >> 8));
    uchar lsb = ((uchar)(0x00FF & num));

    data[0] = static_cast<uchar>(0xAA);
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister) | 0x80;
    data[3] = lsb;
    data[4] = msb;

//    this->serialTransaction(1000, data);
}

batlabStatusInfo Batlab::getInfo()
{
    return info;
}

//void Batlab::serialTransaction(int timeout, const QVector<uchar> request, int sleepAfterTransaction)
//{
//}

//void Batlab::processSerialResponse(const QVector<uchar> response)
//{
//    BatlabLib::debugResponsePacket(info.serialNumberComplete, response);

//    uchar packetStartByte = response[0];
//    uchar packetNamespace = response[1];
//    uchar packetAddress = response[2];
//    uchar packetLowbyte = response[3];
//    uchar packetHighbyte = response[4];

//    // Write response if the address sent was or'ed with 0x80
//    if (packetAddress & 0x80)
//    {

//    }
//    // Read response
//    else
//    {
//        // Cell namespace
//        if (packetNamespace == batlabNamespaces::CHANNEL0 || packetNamespace == batlabNamespaces::CHANNEL1
//                || packetNamespace == batlabNamespaces::CHANNEL2 || packetNamespace == batlabNamespaces::CHANNEL3)
//        {
//            if (packetAddress == cellNamespace::MODE)
//            {

//            }
//            else if (packetAddress == cellNamespace::CELL_ERROR)
//            {

//            }
//            else if (packetAddress == cellNamespace::STATUS)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_SETPOINT)
//            {

//            }
//            else if (packetAddress == cellNamespace::REPORT_INTERVAL)
//            {

//            }
//            else if (packetAddress == cellNamespace::TEMPERATURE)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLTAGE)
//            {

//            }
//            else if (packetAddress == cellNamespace::CHARGEL)
//            {

//            }
//            else if (packetAddress == cellNamespace::CHARGEH)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_CHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_DCHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_LIMIT_CHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_LIMIT_DCHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::TEMP_LIMIT_CHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::TEMP_LIMIT_DCHG)
//            {

//            }
//            else if (packetAddress == cellNamespace::DUTY)
//            {

//            }
//            else if (packetAddress == cellNamespace::COMPENSATION)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_PP)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLTAGE_PP)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_CALIB_OFF)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURRENT_CALIB_SCA)
//            {

//            }
//            else if (packetAddress == cellNamespace::TEMP_CALIB_R)
//            {
//                tempCalibR[packetNamespace] = 256*packetHighbyte + packetLowbyte;
//            }
//            else if (packetAddress == cellNamespace::TEMP_CALIB_B)
//            {
//                tempCalibB[packetNamespace] = 256*packetHighbyte + packetLowbyte;
//            }
//            else if (packetAddress == cellNamespace::CURRENT_CALIB_PP)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLTAGE_CALIB_PP)
//            {

//            }
//            else if (packetAddress == cellNamespace::CURR_CALIB_PP_OFF)
//            {

//            }
//            else if (packetAddress == cellNamespace::VOLT_CALIB_PP_OFF)
//            {

//            }
//            else
//            {
//                qWarning() << "Unknown address in response in CELL namespace.";
//                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//            }
//        }
//        // Unit namespace
//        else if (packetNamespace == batlabNamespaces::UNIT)
//        {
//            if (packetAddress == unitNamespace::SERIAL_NUM)
//            {
//                int newSerial = 256*packetHighbyte + packetLowbyte;
//                if (newSerial != info.serialNumberRegister)
//                {
//                    info.serialNumberRegister = newSerial;
//                    if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
//                    {
//                        info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
//                        emit infoUpdated();
//                    }
//                }
//            }
//            else if (packetAddress == unitNamespace::DEVICE_ID)
//            {
//                int newDeviceId = 256*packetHighbyte + packetLowbyte;
//                if (newDeviceId != info.deviceIdRegister)
//                {
//                    info.deviceIdRegister = newDeviceId;
//                    if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
//                    {
//                        info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
//                        emit infoUpdated();
//                    }
//                }
//            }
//            else if (packetAddress == unitNamespace::FIRMWARE_VER)
//            {
//                int newFirmwareVersion = 256*packetHighbyte + packetLowbyte;
//                if (newFirmwareVersion != info.firmwareVersion)
//                {
//                    info.firmwareVersion = newFirmwareVersion;
//                    emit infoUpdated();
//                }
//            }
//            else if (packetAddress == unitNamespace::VCC)
//            {

//            }
//            else if (packetAddress == unitNamespace::SINE_FREQ)
//            {

//            }
//            else if (packetAddress == unitNamespace::SYSTEM_TIMER)
//            {

//            }
//            else if (packetAddress == unitNamespace::SETTINGS)
//            {

//            }
//            else if (packetAddress == unitNamespace::SINE_OFFSET)
//            {

//            }
//            else if (packetAddress == unitNamespace::SINE_MAGDIV)
//            {

//            }
//            else if (packetAddress == unitNamespace::LED_MESSAGE)
//            {

//            }
//            else if (packetAddress == unitNamespace::UNIT_BOOTLOAD)
//            {

//            }
//            else if (packetAddress == unitNamespace::VOLT_CH_CALIB_OFF)
//            {

//            }
//            else if (packetAddress == unitNamespace::VOLT_CH_CALIB_SCA)
//            {

//            }
//            else if (packetAddress == unitNamespace::VOLT_DC_CALIB_OFF)
//            {

//            }
//            else if (packetAddress == unitNamespace::VOLT_DC_CALIB_SCA)
//            {

//            }
//            else if (packetAddress == unitNamespace::LOCK)
//            {

//            }
//            else
//            {
//                qWarning() << "Unknown address in response in UNIT namespace.";
//                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//            }
//        }
//        else if (packetNamespace == batlabNamespaces::BOOTLOADER)
//        {
//            if (packetAddress == bootloaderNamespace::BOOTLOAD)
//            {

//            }
//            else if (packetAddress == bootloaderNamespace::ADDR)
//            {

//            }
//            else if (packetAddress == bootloaderNamespace::DATA)
//            {

//            }
//            else
//            {
//                qWarning() << "Unknown address in response in BOOTLOADER namespace.";
//                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//            }
//        }
//        else if (packetNamespace == batlabNamespaces::COMMS)
//        {
//            if (packetAddress == commsNamespace::LED0)
//            {

//            }
//            else if (packetAddress == commsNamespace::LED1)
//            {

//            }
//            else if (packetAddress == commsNamespace::LED2)
//            {

//            }
//            else if (packetAddress == commsNamespace::LED3)
//            {

//            }
//            else if (packetAddress == commsNamespace::EXTERNAL_PSU)
//            {
//                bool newExtPSU = 256*packetHighbyte + packetLowbyte;
//                if (newExtPSU != info.externalPowerConnected)
//                {
//                    info.externalPowerConnected = newExtPSU;
//                    emit infoUpdated();
//                }
//            }
//            else if (packetAddress == commsNamespace::EXTERNAL_PSU_VOLTAGE)
//            {

//            }
//            else
//            {
//                qWarning() << "Unknown address in response in COMMS namespace.";
//                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//            }
//        }
//        else
//        {
//            qWarning() << "Unknown namespace in response packet.";
//            BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//        }
//    }
//}

//void Batlab::processSerialError(const QString &s)
//{
//    qWarning() << s;
//}

//void Batlab::processSerialTimeout(const QString &s)
//{
//    qWarning() << s;
//}

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

    qDebug() << tr("Batlab connected to port %1 entering bootloader").arg(info.portName);

    // Enter bootloader
    QQueue<BatlabPacket> packets;
    BatlabPacket enterBootloaderPacket = BatlabPacket(batlabNamespaces::UNIT, unitNamespace::UNIT_BOOTLOAD, 0x00, 0x00);
    enterBootloaderPacket.setSleepAfterTransaction_ms(2000);
    packets.append(enterBootloaderPacket);

    // LEFT OFF

    // ctr = 0x0400
    int ctr = 0x0400;

    // for each byte in the file
        // self.write(BOOTLOADER,BL_ADDR,int(ctr))
        // self.write(BOOTLOADER,BL_DATA,int(ord(byte)))
        // bb = self.read(BOOTLOADER,BL_DATA).value()
        // verify bb == int(ord(byte))
        // print str(ctr - 0x03FF) of FIRMWARE_SIZE: str(bb)
        // ctr++


    // reboot into new image
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
