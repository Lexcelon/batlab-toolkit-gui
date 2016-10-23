#include "batlabcom.h"


batlabCom::batlabCom(QObject *parent) : QObject(parent)
{
    port = new QSerialPort();
    QList<QSerialPortInfo> list = QSerialPortInfo::availablePorts();
    for (int i = 0; i< list.size(); ++i) {
        qDebug() << list[i].portName();
    }
    port->setPortName("COM4");
    port->setBaudRate(QSerialPort::Baud115200);
    bool success = port->open(QSerialPort::ReadWrite);
    if (!success) {
        qDebug() << "Failure Opening Port";
    }

     connect(port,SIGNAL(readyRead()),this,SLOT(onRead()));
//    char * data = new char[50];
//    int i;
//    for (i = 0; i < 7; ++i) {
//        data[0+5*i] = 0xAA;
//        data[1+5*i] = (0x00<<2) + 0x00;
//        data[2+5*i] = 0x02+i;
//        data[3+5*i] = 0x00;
//        data[4+5*i] = 0x00;
//       // qDebug() << uchar(data[0]) << uchar(data[1]) << uchar(data[2]) << uchar(data[3]) << uchar(data[4]);
//    }
//    port->write(data,5*(i));
//    bool truth = port->waitForBytesWritten(1000);
//    if (truth)
//    qDebug() << "WRITTEN";
//    else
//        qDebug() << "NOT WRITTEN";

//    while (port->waitForReadyRead(1000)) {
//        ;
//    }


    //port->close();
}

void batlabCom::onRead() {
    char * rec = new char[100];
    qint64 len = port->bytesAvailable();
    int start = 0;
    port->read(rec,len);
//    for (int i = 0; i < (len/5); ++i) {
//        qDebug() << (uchar)rec[0+5*i] << (uchar)rec[1+5*i] << (uchar)(rec[2+5*i] & 0x7F) << (uchar)rec[3+5*i] << (uchar)rec[4+5*i];
//    }
    while (len > 0)
    if ((uchar)rec[start] == 0xAA) {
        QString str;
        qDebug() << "RESPONSE PACKET";
        qDebug() << "Unit: " << (uchar)(rec[start+1] >> 2) << " Cell: " << (uchar)(rec[start+1] & 0x03);
        switch ((uchar)rec[start+2]) {
        case 0x02:
            str = "Temperature";
            qDebug() << "Temperature: ";
            break;
        case 0x03:
            str = "Current";
            qDebug() << "Current: ";
            break;
        case 0x04:
            str = "Current Amplitude";
            qDebug() << "Current Amplitude: ";
            break;
        case 0x05:
            str = "Voltage";
            qDebug() << "Voltage: ";

            break;
        case 0x06:
            str = "Voltage Phase";
            qDebug() << "Voltage Phase: ";
            break;
        case 0x07:
            str = "Voltage Amplitude";
            qDebug() << "Voltage Amplitude: ";
            break;
        case 0x08:
            str = "Energy";
            qDebug() << "Energy: ";
            break;
        case 0x09:
            str = "Charge";
            qDebug() << "Charge: ";

            break;
        default:
            break;
        }
        qDebug() << 256*(uchar)rec[start+3] + (uchar)rec[start+4];
//        qDebug() << (uchar)(rec[start+1] >> 2) << (uchar)(rec[start+1] & 0x03)<<str << 256*(uint)rec[start+3] + (uint)rec[start+4];
        emit emitResponse((int)(rec[start+1] >> 2),(int)(rec[start+1] & 0x03),str, 256*(int)rec[start+3] + (int)rec[start+4]);
        len-=5;
        start +=5;
    } else if (rec[0] == 0xAF) {

    }
//    for (int i = 0; i < len; i++) {
//        qDebug() << (uchar)rec[i];
//    }
//    qDebug() << rec[3]*256+rec[4];


//    qint64 bytes = port->bytesAvailable();
//    for (int i = 0; i < bytes; i++) {
//        data.push_back(port->read(1)[0]);
//    }
//    parseData();
}

void batlabCom::parseData() {

}

batlabCom::~batlabCom()
{

}

void batlabCom::onGetTemp(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x02;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetCurr(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x03;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetCurrAmp(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x04;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetVol(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x05;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetVolPhase(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x06;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetVolAmp(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x07;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetEnergy(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x08;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

void batlabCom::onGetCharge(int unit, int cell) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = (unit<<2) + cell;
    data[2] = 0x09;
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}
