#include "batlabcom.h"


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
    port->setPortName(item);
    port->setBaudRate(QSerialPort::Baud115200);
    bool success = port->open(QSerialPort::ReadWrite);
    if (!success) {
        qDebug() << "Failure Opening Port";
    }

     connect(port,SIGNAL(readyRead()),this,SLOT(onRead()));
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
        qDebug() << "Low Byte Fail: " << (bool)(rec[start+3]) << " High Byte Fail: " << (bool)(rec[start+4]);

        emit emitResponse((int)(rec[start+1]),(int)(rec[start+2]), (int)(rec[start+3]), (int)rec[start+4]);
        len-=5;
        start +=5;
    } else if ((uchar)rec[start] == 0xAF) {
        qDebug() << (int)(rec[start+1]);
        int cell = (int)(rec[start+1]);
        if ((uchar)rec[start+2] == 0x00) {
            qDebug() << "STREAM PACKET";
            qDebug() << "Unit: " << (uchar)(rec[start+1] >> 2) << " Cell: " << (uchar)(rec[start+1] & 0x03);
            int mode, status, temp, current, voltage;
            mode = 256*(uchar)rec[start+3] + (uchar)rec[start+4];
            status = 256*(uchar)rec[start+5] + (uchar)rec[start+6];
            temp = 256*(uchar)rec[start+7] + (uchar)rec[start+8];
            current = 256*(uchar)rec[start+9] + (uchar)rec[start+10];
            voltage = 256*(uchar)rec[start+11] + (uchar)rec[start+12];
            emit emitStream(cell,mode,status,getTemperature(temp),getCurrent(current),getVoltage(voltage));
        }
//        else {
//            qDebug() << "STREAM PACKET EXT";
//            qDebug() << "Unit: " << (uchar)(rec[start+1] >> 2) << " Cell: " << (uchar)(rec[start+1] & 0x03);
//            int currAmp,volPhase,volAmp;
//            currAmp = 256*(uchar)rec[start+3] + (uchar)rec[start+4];
//            volPhase = 256*(uchar)rec[start+5] + (uchar)rec[start+6];
//            volAmp = 256*(uchar)rec[start+7] + (uchar)rec[start+8];
//            emit emitStreamExt(unit,cell,currAmp,volPhase,volAmp);
//        }
        len-=13;
        start+=13;
    }
}

batlabCom::~batlabCom() {
    port->close();
}

void batlabCom::onReadReg(int batlabNamespace, int batlabRegister) {
    char * data = new char[5];
    data[0] = 0xAA;
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister);
    data[3] = 0x00;
    data[4] = 0x00;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}


void batlabCom::onWriteReg(int batlabNamespace, int batlabRegister,int num) {
    char * data = new char[5];
    uchar msb = ((uchar)((0xFF00 & num) >> 8));
    uchar lsb = ((uchar)(0x00FF & num));
    qDebug() << "WRITE MESSAGE";
    qDebug() << "NAMESPACE: " << batlabNamespace << " REGISTER: " << batlabRegister << " VALUE : " << num;
    qDebug() << msb << lsb;
    data[0] = 0xAA;
    data[1] = static_cast<uchar>(batlabNamespace);
    data[2] = static_cast<uchar>(batlabRegister) | 0x80;
    data[3] = msb;
    data[4] = lsb;

    port->write(data,5);
    port->waitForBytesWritten(1000);
}


