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
#include <QFileInfo>

#include "batlablib.h"
#include "batlabcommthread.h"

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
    void initiateFirmwareFlash(QString firmwareFilePath);

    QString getPortName() { return info.portName; }
    int getSerialNumber() {return info.serialNumberRegister; }
    batlabDisplayInfo getInfo();

    void checkSerialPortError();
    void periodicCheck();
    bool hasReceivedValidResponse();

private slots:
    void serialTransaction(int timeout, const QVector<uchar> request);
    void processSerialResponse(const QVector<uchar> response);
    void processSerialError(const QString &s);
    void processSerialTimeout(const QString &s);

private:
    batlabDisplayInfo info;

    int tempCalibB[4];
    int tempCalibR[4];

    BatlabCommThread m_commThread;

    bool m_hasReceivedValidResponse;
};

#endif // BATLABCOM_H
