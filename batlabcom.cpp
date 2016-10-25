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
        qDebug() << names[enumVals[(uchar)rec[start+2]]] +": " << 256*(int)rec[start+3] + (int)rec[start+4];
        qDebug() << (int)rec[start+1];
        emit emitResponse((int)(rec[start+1] >> 2),(int)(rec[start+1] & 0x03),names[enumVals[(uchar)rec[start+2]]], 256*(int)rec[start+3] + (int)rec[start+4]);
        len-=5;
        start +=5;
    } else if ((uchar)rec[start] == 0xAF) {
        qDebug() << "RESPONSE PACKET";
        qDebug() << "Unit: " << (uchar)(rec[start+1] >> 2) << " Cell: " << (uchar)(rec[start+1] & 0x03);
        int unit = (int)(rec[start+1] >> 2);
        int cell = (int)(rec[start+1] & 0x03);
        if ((uchar)rec[start+2] == 0x00) {
            int status,temp,current,voltage,charge;
            status = 256*(uchar)rec[start+3] + (uchar)rec[start+4];
            temp = 256*(uchar)rec[start+5] + (uchar)rec[start+6];
            current = 256*(uchar)rec[start+7] + (uchar)rec[start+8];
            voltage = 256*(uchar)rec[start+9] + (uchar)rec[start+10];
            charge = 256*(uchar)rec[start+11] + (uchar)rec[start+12];
            emit emitStream(unit,cell,status,temp,current,voltage,charge);
        } else {
            int currAmp,volPhase,volAmp;
            currAmp = 256*(uchar)rec[start+3] + (uchar)rec[start+4];
            volPhase = 256*(uchar)rec[start+5] + (uchar)rec[start+6];
            volAmp = 256*(uchar)rec[start+7] + (uchar)rec[start+8];
            emit emitStreamExt(unit,cell,currAmp,volPhase,volAmp);
        }
    }
}

void batlabCom::parseData() {

}

batlabCom::~batlabCom()
{

}

void batlabCom::onReadReg(int unit, int cell, vals val) {
    char * data = new char[5];
    uchar value;
    //qDebug() << registers[val];
    data[0] = 0xAA;
    if (val == numberOfConnectedUnits) {
        value = 0xFF;
    } else if (val == numberOfConnectedCells) {
        value = unit;
    } else {
        value = ((unit<<2) + cell);
    }
    data[1] = value;
    data[2] = registers[val];
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}

