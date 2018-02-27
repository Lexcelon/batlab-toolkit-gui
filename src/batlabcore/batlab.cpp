#include "batlab.h"

Batlab::Batlab(QString newPortName, QObject *parent) : QObject(parent)
{
    info.externalPowerConnected = false;
    info.firmwareVersion = -1;
    info.portName = newPortName;
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

    m_hasReceivedValidResponse = false;

//    port = new QSerialPort();
//    port->setPortName(info.portName);
//    port->setBaudRate(QSerialPort::Baud115200);

//    if (!port->open(QSerialPort::ReadWrite)) {
//        qWarning() << "Failure Opening Port: " << port->error() << port->errorString();
//        return;
//    }

//    connect(port, &QSerialPort::errorOccurred, this, &Batlab::checkSerialPortError);
//    connect(port, &QSerialPort::readyRead, this, &Batlab::processAvailableSerialPortData);

    initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM);
    initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::DEVICE_ID);
    initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::FIRMWARE_VER);

    initiateRegisterRead(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_R);

    initiateRegisterRead(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU);

    QTimer *batlabPeriodicCheckTimer = new QTimer(this);
    connect(batlabPeriodicCheckTimer, &QTimer::timeout, this, &Batlab::periodicCheck);
    batlabPeriodicCheckTimer->start(5000);

    connect(&m_commThread, &BatlabCommThread::response, this, &Batlab::processResponse);
    connect(&m_commThread, &BatlabCommThread::error, this, &Batlab::processError);
    connect(&m_commThread, &BatlabCommThread::timeout, this, &Batlab::processTimeout);
}

void Batlab::periodicCheck()
{
    if (info.serialNumberRegister == -1)
    {
        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM);
    }
    if (info.deviceIdRegister == -1)
    {
        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::DEVICE_ID);
    }
    if (info.firmwareVersion == -1)
    {
        initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::FIRMWARE_VER);
    }
    for (int i = 0; i < 4; i++)
    {
        if (tempCalibB[i] == -1)
        {
            initiateRegisterRead(i, cellNamespace::TEMP_CALIB_B);
        }
        if (tempCalibR[i] == -1)
        {
            initiateRegisterRead(i, cellNamespace::TEMP_CALIB_R);
        }
    }

    initiateRegisterRead(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU);
}

// TODO maybe not remove but need a function for when unexpected data (usually stream) arrives
// TODO move this into comm thread?
void Batlab::processAvailableSerialPortData() {
//    qint64 dataLength = port->bytesAvailable();
//    char *data = new char[dataLength];
//    int startChar = 0;
//    port->read(data, dataLength);

//    while (dataLength > 0) {
//        // This is a response
//        uchar packetStartByte = data[startChar];
//        if (packetStartByte == 0xAA) {

//            uchar packetNamespace = data[startChar+1];
//            uchar packetAddress = data[startChar+2];
//            uchar packetLowbyte = data[startChar+3];
//            uchar packetHighbyte = data[startChar+4];

//            // Write response if the address sent was or'ed with 0x80
//            if (packetAddress & 0x80)
//            {

//            }
//            // Read response
//            else
//            {
//                // Cell namespace
//                if (packetNamespace == batlabNamespaces::CHANNEL0 || packetNamespace == batlabNamespaces::CHANNEL1
//                        || packetNamespace == batlabNamespaces::CHANNEL2 || packetNamespace == batlabNamespaces::CHANNEL3)
//                {
//                    if (packetAddress == cellNamespace::MODE)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CELL_ERROR)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::STATUS)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_SETPOINT)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::REPORT_INTERVAL)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::TEMPERATURE)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLTAGE)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CHARGEL)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CHARGEH)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_CHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_DCHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_LIMIT_CHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_LIMIT_DCHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::TEMP_LIMIT_CHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::TEMP_LIMIT_DCHG)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::DUTY)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::COMPENSATION)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_PP)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLTAGE_PP)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_CALIB_OFF)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_CALIB_SCA)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::TEMP_CALIB_R)
//                    {
//                        tempCalibR[packetNamespace] = 256*packetHighbyte + packetLowbyte;
//                    }
//                    else if (packetAddress == cellNamespace::TEMP_CALIB_B)
//                    {
//                        tempCalibB[packetNamespace] = 256*packetHighbyte + packetLowbyte;
//                    }
//                    else if (packetAddress == cellNamespace::CURRENT_CALIB_PP)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLTAGE_CALIB_PP)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::CURR_CALIB_PP_OFF)
//                    {

//                    }
//                    else if (packetAddress == cellNamespace::VOLT_CALIB_PP_OFF)
//                    {

//                    }
//                    else
//                    {
//                        qWarning() << "Unknown address in response in CELL namespace.";
//                        BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//                    }
//                }
//                // Unit namespace
//                else if (packetNamespace == batlabNamespaces::UNIT)
//                {
//                    if (packetAddress == unitNamespace::SERIAL_NUM)
//                    {
//                        int newSerial = 256*packetHighbyte + packetLowbyte;
//                        if (newSerial != info.serialNumberRegister)
//                        {
//                            info.serialNumberRegister = newSerial;
//                            if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
//                            {
//                                info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
//                                emit infoUpdated();
//                            }
//                        }
//                    }
//                    else if (packetAddress == unitNamespace::DEVICE_ID)
//                    {
//                        int newDeviceId = 256*packetHighbyte + packetLowbyte;
//                        if (newDeviceId != info.deviceIdRegister)
//                        {
//                            info.deviceIdRegister = newDeviceId;
//                            if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
//                            {
//                                info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
//                                emit infoUpdated();
//                            }
//                        }
//                    }
//                    else if (packetAddress == unitNamespace::FIRMWARE_VER)
//                    {
//                        int newFirmwareVersion = 256*packetHighbyte + packetLowbyte;
//                        if (newFirmwareVersion != info.firmwareVersion)
//                        {
//                            info.firmwareVersion = newFirmwareVersion;
//                            emit infoUpdated();
//                        }
//                    }
//                    else if (packetAddress == unitNamespace::VCC)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::SINE_FREQ)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::SYSTEM_TIMER)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::SETTINGS)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::SINE_OFFSET)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::SINE_MAGDIV)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::LED_MESSAGE)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::UNIT_BOOTLOAD)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::VOLT_CH_CALIB_OFF)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::VOLT_CH_CALIB_SCA)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::VOLT_DC_CALIB_OFF)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::VOLT_DC_CALIB_SCA)
//                    {

//                    }
//                    else if (packetAddress == unitNamespace::LOCK)
//                    {

//                    }
//                    else
//                    {
//                        qWarning() << "Unknown address in response in UNIT namespace.";
//                        BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//                    }
//                }
//                else if (packetNamespace == batlabNamespaces::BOOTLOADER)
//                {
//                    if (packetAddress == bootloaderNamespace::BOOTLOAD)
//                    {

//                    }
//                    else if (packetAddress == bootloaderNamespace::ADDR)
//                    {

//                    }
//                    else if (packetAddress == bootloaderNamespace::DATA)
//                    {

//                    }
//                    else
//                    {
//                        qWarning() << "Unknown address in response in BOOTLOADER namespace.";
//                        BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//                    }
//                }
//                else if (packetNamespace == batlabNamespaces::COMMS)
//                {
//                    if (packetAddress == commsNamespace::LED0)
//                    {

//                    }
//                    else if (packetAddress == commsNamespace::LED1)
//                    {

//                    }
//                    else if (packetAddress == commsNamespace::LED2)
//                    {

//                    }
//                    else if (packetAddress == commsNamespace::LED3)
//                    {

//                    }
//                    else if (packetAddress == commsNamespace::EXTERNAL_PSU)
//                    {
//                        bool newExtPSU = 256*packetHighbyte + packetLowbyte;
//                        if (newExtPSU != info.externalPowerConnected)
//                        {
//                            info.externalPowerConnected = newExtPSU;
//                            emit infoUpdated();
//                        }
//                    }
//                    else if (packetAddress == commsNamespace::EXTERNAL_PSU_VOLTAGE)
//                    {

//                    }
//                    else
//                    {
//                        qWarning() << "Unknown address in response in COMMS namespace.";
//                        BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//                    }
//                }
//                else
//                {
//                    qWarning() << "Unknown namespace in response packet.";
//                    BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
//                }

//            }

//            dataLength-=5;
//            startChar +=5;
//        } else if (packetStartByte == 0xAF) {

//            int cell = static_cast<int>(data[startChar+1]);
//            if ((uchar)data[startChar+2] == 0x00) {
//                int mode, status, temp, current, voltage;
//                mode =      (uchar)data[startChar+3]  + 256*(uchar)data[startChar+4];
//                status =    (uchar)data[startChar+5]  + 256*(uchar)data[startChar+6];
//                temp =      (uchar)data[startChar+7]  + 256*(uchar)data[startChar+8];
//                current =   (uchar)data[startChar+9]  + 256*(uchar)data[startChar+10];
//                voltage =   (uchar)data[startChar+11] + 256*(uchar)data[startChar+12];
//                emit newStreamReceived(cell,mode,status,BatlabLib::getTemp(temp,tempCalibB[cell],tempCalibR[cell]),BatlabLib::getCurrent(current),BatlabLib::getVoltage(voltage));
//            }
//            dataLength-=13;
//            startChar+=13;
//        }
//    }
}

Batlab::~Batlab()
{
}

void Batlab::setAllIdle()
{
    for (int i = 0; i < 4; ++i) {
        initiateRegisterWrite(i, cellNamespace::MODE, MODE_IDLE);
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

    this->transaction(1000, data);
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

    this->transaction(1000, data);
}

// TODO do this in the comm thread
void Batlab::checkSerialPortError() {

    // ResourceError indicates that the Batlab was disconnected
//    if (port->error() == QSerialPort::ResourceError) {
//        emit batlabDisconnected(info.portName);
//    }

}

batlabInfo Batlab::getInfo()
{
    return info;
}

void Batlab::transaction(int timeout, const QVector<uchar> request)
{
    m_commThread.transaction(info.serialNumberComplete, info.portName, timeout, request);
}

void Batlab::processResponse(const QVector<uchar> response)
{
    BatlabLib::debugResponsePacket(info.serialNumberComplete, response);

    m_hasReceivedValidResponse = true;

    uchar packetStartByte = response[0];
    uchar packetNamespace = response[1];
    uchar packetAddress = response[2];
    uchar packetLowbyte = response[3];
    uchar packetHighbyte = response[4];

    // Write response if the address sent was or'ed with 0x80
    if (packetAddress & 0x80)
    {

    }
    // Read response
    else
    {
        // Cell namespace
        if (packetNamespace == batlabNamespaces::CHANNEL0 || packetNamespace == batlabNamespaces::CHANNEL1
                || packetNamespace == batlabNamespaces::CHANNEL2 || packetNamespace == batlabNamespaces::CHANNEL3)
        {
            if (packetAddress == cellNamespace::MODE)
            {

            }
            else if (packetAddress == cellNamespace::CELL_ERROR)
            {

            }
            else if (packetAddress == cellNamespace::STATUS)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_SETPOINT)
            {

            }
            else if (packetAddress == cellNamespace::REPORT_INTERVAL)
            {

            }
            else if (packetAddress == cellNamespace::TEMPERATURE)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT)
            {

            }
            else if (packetAddress == cellNamespace::VOLTAGE)
            {

            }
            else if (packetAddress == cellNamespace::CHARGEL)
            {

            }
            else if (packetAddress == cellNamespace::CHARGEH)
            {

            }
            else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_CHG)
            {

            }
            else if (packetAddress == cellNamespace::VOLTAGE_LIMIT_DCHG)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_LIMIT_CHG)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_LIMIT_DCHG)
            {

            }
            else if (packetAddress == cellNamespace::TEMP_LIMIT_CHG)
            {

            }
            else if (packetAddress == cellNamespace::TEMP_LIMIT_DCHG)
            {

            }
            else if (packetAddress == cellNamespace::DUTY)
            {

            }
            else if (packetAddress == cellNamespace::COMPENSATION)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_PP)
            {

            }
            else if (packetAddress == cellNamespace::VOLTAGE_PP)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_CALIB_OFF)
            {

            }
            else if (packetAddress == cellNamespace::CURRENT_CALIB_SCA)
            {

            }
            else if (packetAddress == cellNamespace::TEMP_CALIB_R)
            {
                tempCalibR[packetNamespace] = 256*packetHighbyte + packetLowbyte;
            }
            else if (packetAddress == cellNamespace::TEMP_CALIB_B)
            {
                tempCalibB[packetNamespace] = 256*packetHighbyte + packetLowbyte;
            }
            else if (packetAddress == cellNamespace::CURRENT_CALIB_PP)
            {

            }
            else if (packetAddress == cellNamespace::VOLTAGE_CALIB_PP)
            {

            }
            else if (packetAddress == cellNamespace::CURR_CALIB_PP_OFF)
            {

            }
            else if (packetAddress == cellNamespace::VOLT_CALIB_PP_OFF)
            {

            }
            else
            {
                qWarning() << "Unknown address in response in CELL namespace.";
                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
            }
        }
        // Unit namespace
        else if (packetNamespace == batlabNamespaces::UNIT)
        {
            if (packetAddress == unitNamespace::SERIAL_NUM)
            {
                int newSerial = 256*packetHighbyte + packetLowbyte;
                if (newSerial != info.serialNumberRegister)
                {
                    info.serialNumberRegister = newSerial;
                    if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
                    {
                        info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
                        emit infoUpdated();
                    }
                }
            }
            else if (packetAddress == unitNamespace::DEVICE_ID)
            {
                int newDeviceId = 256*packetHighbyte + packetLowbyte;
                if (newDeviceId != info.deviceIdRegister)
                {
                    info.deviceIdRegister = newDeviceId;
                    if (info.serialNumberRegister != -1 && info.deviceIdRegister != -1)
                    {
                        info.serialNumberComplete = (info.deviceIdRegister<<16) + info.serialNumberRegister;
                        emit infoUpdated();
                    }
                }
            }
            else if (packetAddress == unitNamespace::FIRMWARE_VER)
            {
                int newFirmwareVersion = 256*packetHighbyte + packetLowbyte;
                if (newFirmwareVersion != info.firmwareVersion)
                {
                    info.firmwareVersion = newFirmwareVersion;
                    emit infoUpdated();
                }
            }
            else if (packetAddress == unitNamespace::VCC)
            {

            }
            else if (packetAddress == unitNamespace::SINE_FREQ)
            {

            }
            else if (packetAddress == unitNamespace::SYSTEM_TIMER)
            {

            }
            else if (packetAddress == unitNamespace::SETTINGS)
            {

            }
            else if (packetAddress == unitNamespace::SINE_OFFSET)
            {

            }
            else if (packetAddress == unitNamespace::SINE_MAGDIV)
            {

            }
            else if (packetAddress == unitNamespace::LED_MESSAGE)
            {

            }
            else if (packetAddress == unitNamespace::UNIT_BOOTLOAD)
            {

            }
            else if (packetAddress == unitNamespace::VOLT_CH_CALIB_OFF)
            {

            }
            else if (packetAddress == unitNamespace::VOLT_CH_CALIB_SCA)
            {

            }
            else if (packetAddress == unitNamespace::VOLT_DC_CALIB_OFF)
            {

            }
            else if (packetAddress == unitNamespace::VOLT_DC_CALIB_SCA)
            {

            }
            else if (packetAddress == unitNamespace::LOCK)
            {

            }
            else
            {
                qWarning() << "Unknown address in response in UNIT namespace.";
                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
            }
        }
        else if (packetNamespace == batlabNamespaces::BOOTLOADER)
        {
            if (packetAddress == bootloaderNamespace::BOOTLOAD)
            {

            }
            else if (packetAddress == bootloaderNamespace::ADDR)
            {

            }
            else if (packetAddress == bootloaderNamespace::DATA)
            {

            }
            else
            {
                qWarning() << "Unknown address in response in BOOTLOADER namespace.";
                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
            }
        }
        else if (packetNamespace == batlabNamespaces::COMMS)
        {
            if (packetAddress == commsNamespace::LED0)
            {

            }
            else if (packetAddress == commsNamespace::LED1)
            {

            }
            else if (packetAddress == commsNamespace::LED2)
            {

            }
            else if (packetAddress == commsNamespace::LED3)
            {

            }
            else if (packetAddress == commsNamespace::EXTERNAL_PSU)
            {
                bool newExtPSU = 256*packetHighbyte + packetLowbyte;
                if (newExtPSU != info.externalPowerConnected)
                {
                    info.externalPowerConnected = newExtPSU;
                    emit infoUpdated();
                }
            }
            else if (packetAddress == commsNamespace::EXTERNAL_PSU_VOLTAGE)
            {

            }
            else
            {
                qWarning() << "Unknown address in response in COMMS namespace.";
                BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
            }
        }
        else
        {
            qWarning() << "Unknown namespace in response packet.";
            BatlabLib::debugResponsePacket(info.serialNumberComplete, packetStartByte, packetNamespace, packetAddress, packetLowbyte, packetHighbyte);
        }
    }
}

void Batlab::processError(const QString &s)
{
    qWarning() << s;
}

void Batlab::processTimeout(const QString &s)
{
    qWarning() << s;
}

bool Batlab::hasReceivedValidResponse()
{
    return m_hasReceivedValidResponse;
}
