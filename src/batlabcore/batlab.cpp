#include "batlab.h"

Batlab::Batlab(QString newPortName, QObject *parent) : QObject(parent)
{
    port = new QSerialPort();
    info.portName = newPortName;
    port->setPortName(info.portName);
    port->setBaudRate(QSerialPort::Baud115200);

    if (!port->open(QSerialPort::ReadWrite)) {
        qWarning() << "Failure Opening Port: " << port->error() << port->errorString();
        return;
    }

    connect(port, &QSerialPort::errorOccurred, this, &Batlab::checkSerialPortError);
    connect(port, &QSerialPort::readyRead, this, &Batlab::processAvailableSerialPortData);

    initiateRegisterRead(batlabNamespaces::UNIT, unitNamespace::SERIAL_NUM);
    initiateRegisterRead(batlabNamespaces::COMMS, commsNamespace::EXTERNAL_PSU);

    initiateRegisterRead(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_B);
    initiateRegisterRead(batlabNamespaces::CHANNEL0, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL1, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL2, cellNamespace::TEMP_CALIB_R);
    initiateRegisterRead(batlabNamespaces::CHANNEL3, cellNamespace::TEMP_CALIB_R);
}

void Batlab::processAvailableSerialPortData() {
    qint64 dataLength = port->bytesAvailable();
    char *data = new char[dataLength];
    int startChar = 0;
    port->read(data, dataLength);

    while (dataLength > 0) {
        // This is a response
        if ((uchar)data[startChar] == 0xAA) {
            qDebug() << "RESPONSE PACKET";
            qDebug() << "Namespace: " << (uchar)(data[startChar+1]) << " Cell: " << (uchar)(data[startChar+2]);
            qDebug() << "Low Byte Fail: " << (int)(data[startChar+3]) << " High Byte Fail: " << (int)(data[startChar+4]);

            if (data[startChar+2] & 0x80) // Write response if the address sent was or'ed with 0x80
            {
                emit emitWriteResponse(static_cast<int>((uchar)data[startChar+1]), static_cast<int>((uchar)data[startChar+2]) ^ 0x0080, static_cast<int>(data[startChar+3]), static_cast<int>(data[startChar+4]));

            }
            else // Read response
            {
                // Check the namespace
                uchar ns = data[startChar+1];
                // Cell namespace
                if (ns == batlabNamespaces::CHANNEL0 || ns == batlabNamespaces::CHANNEL1 || ns == batlabNamespaces::CHANNEL2 || ns == batlabNamespaces::CHANNEL3)
                {

                }
                // Unit namespace
                else if (ns == batlabNamespaces::UNIT)
                {

                }

                else if (data[startChar+2] == cellNamespace::TEMP_CALIB_R) {
                    tempCalibR[(uchar)(data[startChar+1])] = 256*(uchar)data[startChar+4] + (uchar)data[startChar+3];
                }
                emit emitReadResponse(static_cast<int>((uchar)data[startChar+1]), static_cast<int>((uchar)data[startChar+2]), static_cast<int>(data[startChar+3]), static_cast<int>(data[startChar+4]));
            }

            dataLength-=5;
            startChar +=5;
        } else if ((uchar)data[startChar] == 0xAF) {

            int cell = static_cast<int>(data[startChar+1]);
            if ((uchar)data[startChar+2] == 0x00) {
                int mode, status, temp, current, voltage;
                mode =      (uchar)data[startChar+3]  + 256*(uchar)data[startChar+4];
                status =    (uchar)data[startChar+5]  + 256*(uchar)data[startChar+6];
                temp =      (uchar)data[startChar+7]  + 256*(uchar)data[startChar+8];
                current =   (uchar)data[startChar+9]  + 256*(uchar)data[startChar+10];
                voltage =   (uchar)data[startChar+11] + 256*(uchar)data[startChar+12];
                emit newStreamReceived(cell,mode,status,BatlabLib::getTemp(temp,tempCalibB[cell],tempCalibR[cell]),BatlabLib::getCurrent(current),BatlabLib::getVoltage(voltage));
            }
            dataLength-=13;
            startChar+=13;
        }
    }
}

Batlab::~Batlab() {
    port->close();
}

void Batlab::setAllIdle()
{
    for (int i = 0; i < 4; ++i) {
        initiateRegisterWrite(i, cellNamespace::MODE, MODE_IDLE);
    }
}

void Batlab::initiateRegisterRead(int batlabNamespace, int batlabRegister)
{
    emit registerReadInitiated(info.serialNumber, batlabNamespace, batlabRegister);

    char *data = new char[5];
    data[0] = static_cast<uchar>(0xAA);
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister);
    data[3] = static_cast<uchar>(0x00);
    data[4] = static_cast<uchar>(0x00);

    port->write(data, 5);
    port->waitForBytesWritten(1000);
}


void Batlab::initiateRegisterWrite(int batlabNamespace, int batlabRegister, int num)
{
    emit registerWriteInitiated(info.serialNumber, batlabNamespace, batlabRegister, num);
    char *data = new char[5];
    uchar msb = ((uchar)((0xFF00 & num) >> 8));
    uchar lsb = ((uchar)(0x00FF & num));
    qDebug() << "WRITE MESSAGE";
    qDebug() << "NAMESPACE: " << batlabNamespace << " REGISTER: " << batlabRegister << " VALUE : " << ushort(num);
    qDebug() << msb << lsb;
    data[0] = static_cast<uchar>(0xAA);
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister) | 0x80;
    data[3] = lsb;
    data[4] = msb;

    port->write(data, 5);
    port->waitForBytesWritten(1000);
}

void Batlab::checkSerialPortError() {

    // ResourceError indicates that the Batlab was disconnected
    if (port->error() == QSerialPort::ResourceError) {
        emit batlabDisconnected(info.portName);
    }

}

batlabInfo Batlab::getInfo()
{
    return info;
}
