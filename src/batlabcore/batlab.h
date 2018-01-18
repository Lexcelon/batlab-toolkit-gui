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
    void emitStream(int,int,int,float,float,float);
    void registerReadInitiated (int, int, int);
    void registerWriteInitiated(int, int, int, int);

    void batlabDisconnected(QString);


public slots:
    void processAvailableSerialPortData();
    void initiateRegisterRead(int, int);
    void initiateRegisterWrite(int, int, int);
    QString getName() { return portName; }
    int getSerialNumber() {return serialNumber; }

    void checkSerialPortError(); //Status for unit port that emits the disconnect signal. Called when port error is detected.

private:
    QSerialPort *port;
    QString portName;
    QVector<uchar> data;

    int serialNumber = -1;

    int tempCalibB[4];
    int tempCalibR[4];
};

#endif // BATLABCOM_H
