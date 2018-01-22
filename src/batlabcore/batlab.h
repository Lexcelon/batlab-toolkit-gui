#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QVector>
#include <QDebug>
#include <QComboBox>
#include <QInputDialog>
#include <qmath.h>
#include "batlablib.h"

class Batlab : public QObject
{
    Q_OBJECT
public:
    Batlab(QString item, QObject *parent = 0);
    ~Batlab();

    void setAllIdle();

signals:
    void newStreamReceived(int,int,int,float,float,float);
    void registerReadInitiated (int, int, int);
    void registerWriteInitiated(int, int, int, int);

    void batlabDisconnected(QString);
    void infoUpdated();

public slots:
    void processAvailableSerialPortData();
    void initiateRegisterRead(int, int);
    void initiateRegisterWrite(int, int, int);
    QString getPortName() { return info.portName; }
    int getSerialNumber() {return info.serialNumber; }
    batlabInfo getInfo();

    void checkSerialPortError();
    void periodicCheck();
    void debugResponsePacket(uchar packetStartByte, uchar packetNamespace, uchar packetAddress, uchar packetLowByte, uchar packetHighByte);

private:
    batlabInfo info;

    QSerialPort *port;
    QVector<uchar> data;

    int tempCalibB[4];
    int tempCalibR[4];
};

#endif // BATLABCOM_H
