#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
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
    void emitReadResponse(int,int,int,int);
    void emitWriteResponse(int,int,int,int);
    void newStreamReceived(int,int,int,float,float,float);
    void registerReadInitiated (int, int, int);
    void registerWriteInitiated(int, int, int, int);

    void batlabDisconnected(QString);
    void infoUpdated();

public slots:
    void processAvailableSerialPortData();
    void initiateRegisterRead(int, int);
    void initiateRegisterWrite(int, int, int);
    QString getName() { return info.portName; }
    int getSerialNumber() {return info.serialNumber; }
    batlabInfo getInfo();

    void checkSerialPortError(); //Status for unit port that emits the disconnect signal. Called when port error is detected.

private:
    batlabInfo info;

    QSerialPort *port;
    QVector<uchar> data;

    int tempCalibB[4];
    int tempCalibR[4];
};

#endif // BATLABCOM_H
