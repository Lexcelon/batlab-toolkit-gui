#include "batlabcom.h"
#include "globals.h"

batlabCom::batlabCom(QObject *parent) : QObject(parent) {
    port = new QSerialPort();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    QStringList names;
    for (int i = 0; i< list.size(); ++i) {
        qDebug() << list[i].portName();
        names.append(list[i].portName());
    }
    bool ok;
    QString item = QInputDialog::getItem(nullptr,"Com Port Selection","Port Name: ",names,0,false,&ok);
    port->setPortName(item);
    port->setBaudRate(QSerialPort::Baud115200);
    bool success = port->open(QSerialPort::ReadWrite);
    if (!success) {
        qDebug() << "Failure Opening Port";
    }

     connect(port,SIGNAL(readyRead()),this,SLOT(onRead()));
}


batlabCom::batlabCom(QString item, QObject *parent) : QObject(parent)
{
    port = new QSerialPort();
    portName = item;
    port->setPortName(item);
    port->setBaudRate(QSerialPort::Baud115200);
    bool success = port->open(QSerialPort::ReadWrite);
    if (!success) {
        qDebug() << "Failure Opening Port";
    }

    connect(port,SIGNAL(readyRead()),this,SLOT(onRead()));
    connect(port,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(checkCommPortStatus()));

    qDebug() << "Read Serial Number";
    onReadReg(0x04,unitNamespace::SERIAL_NUM);
    onReadReg(0x00,cellNamespace::TEMP_CALIB_B);
    onReadReg(0x01,cellNamespace::TEMP_CALIB_B);
    onReadReg(0x02,cellNamespace::TEMP_CALIB_B);
    onReadReg(0x03,cellNamespace::TEMP_CALIB_B);
    onReadReg(0x00,cellNamespace::TEMP_CALIB_R);
    onReadReg(0x01,cellNamespace::TEMP_CALIB_R);
    onReadReg(0x02,cellNamespace::TEMP_CALIB_R);
    onReadReg(0x03,cellNamespace::TEMP_CALIB_R);
}

void batlabCom::onRead() {
    qint64 len = port->bytesAvailable();
    char * rec = new char[len];
    int start = 0;
    port->read(rec,len);

    while (len > 0)
    if ((uchar)rec[start] == 0xAA) {
        qDebug() << "RESPONSE PACKET";
        qDebug() << "Namespace: " << (uchar)(rec[start+1]) << " Cell: " << (uchar)(rec[start+2]);
        qDebug() << "Low Byte Fail: " << (int)(rec[start+3]) << " High Byte Fail: " << (int)(rec[start+4]);

        if (rec[start+2] & 0x80) {
            emit emitWriteResponse(static_cast<int>((uchar)rec[start+1]), static_cast<int>((uchar)rec[start+2]) ^ 0x0080, static_cast<int>(rec[start+3]), static_cast<int>(rec[start+4]));
        } else {
            if (rec[start+2] == unitNamespace::SERIAL_NUM) {
                serialNumber = 256*(uchar)rec[start+4] + (uchar)rec[start+3];
                qDebug() << "Serial  Number " << serialNumber;
            }

            if (rec[start+2] == cellNamespace::TEMP_CALIB_B) {
                tempCalibB[(uchar)(rec[start+1])] = 256*(uchar)rec[start+4] + (uchar)rec[start+3];
            }

            if (rec[start+2] == cellNamespace::TEMP_CALIB_R) {
                tempCalibR[(uchar)(rec[start+1])] = 256*(uchar)rec[start+4] + (uchar)rec[start+3];
            }
            emit emitReadResponse(static_cast<int>((uchar)rec[start+1]), static_cast<int>((uchar)rec[start+2]), static_cast<int>(rec[start+3]), static_cast<int>(rec[start+4]));
        }

        len-=5;
        start +=5;
    } else if ((uchar)rec[start] == 0xAF) {
//        qDebug() << static_cast<int>(rec[start+1])
//                << static_cast<int>(rec[start+2])
//                << static_cast<int>(rec[start+3])
//                << static_cast<int>(rec[start+4])
//                << static_cast<int>(rec[start+5])
//                << static_cast<int>(rec[start+6])
//                << static_cast<int>(rec[start+7])
//                << static_cast<int>(rec[start+8])
//                << static_cast<int>(rec[start+9])
//                << static_cast<int>(rec[start+10])
//                << static_cast<int>(rec[start+11])
//                << static_cast<int>(rec[start+12]);

        int cell = static_cast<int>(rec[start+1]);
        if ((uchar)rec[start+2] == 0x00) {
            int mode, status, temp, current, voltage;
            mode =      (uchar)rec[start+3]  + 256*(uchar)rec[start+4];
            status =    (uchar)rec[start+5]  + 256*(uchar)rec[start+6];
            temp =      (uchar)rec[start+7]  + 256*(uchar)rec[start+8];
            current =   (uchar)rec[start+9]  + 256*(uchar)rec[start+10];
            voltage =   (uchar)rec[start+11] + 256*(uchar)rec[start+12];
            emit emitStream(cell,mode,status,BatlabLib::getTemp(temp,tempCalibB[cell],tempCalibR[cell]),BatlabLib::getCurrent(current),BatlabLib::getVoltage(voltage));
        }
        len-=13;
        start+=13;
    }
}

batlabCom::~batlabCom() {
    port->close();
}

void batlabCom::setAllIdle()
{
    for (int i = 0; i < 4; ++i) {
        onWriteReg(i, cellNamespace::MODE, MODE_IDLE);
    }
}

void batlabCom::onReadReg(int batlabNamespace, int batlabRegister)
{
    emit emitReadCommand(serialNumber, batlabNamespace, batlabRegister);

    char * data = new char[5];
    data[0] = static_cast<uchar>(0xAA);
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister);
    data[3] = static_cast<uchar>(0x00);
    data[4] = static_cast<uchar>(0x00);

    port->write(data,5);
    port->waitForBytesWritten(1000);
}


void batlabCom::onWriteReg(int batlabNamespace, int batlabRegister, int num)
{
    emit emitWriteCommand(serialNumber, batlabNamespace, batlabRegister, num);
    char * data = new char[5];
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

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::checkCommPortStatus() {

    // Value of 9 indicates that Batlab was disconnected.
    if (port->QSerialPort::error() == 9) {
        emit emitBatlabDisconnect(portName);
    }

}
