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
#include <QQueue>
#include <QStateMachine>

#include "batlablib.h"
#include "channel.h"
#include "batlabcommsmanager.h"

class Batlab : public QObject
{
    Q_OBJECT
public:
    Batlab(QString portName, QObject *parent = 0);
    ~Batlab();

    void setAllIdle();

signals:
    void newStreamReceived(int,int,int,float,float,float);
    void registerReadInitiated (int, int, int);
    void registerWriteInitiated(int, int, int, int);

    void bootloaderEntered();

    void batlabDisconnected(QString);
    void infoUpdated();

    void error(const QString &s);

public slots:
    void initiateRegisterRead(int, int);
    void initiateRegisterWrite(int, int, int);
    void initiateFirmwareFlash(QString firmwareFilePath);

    QString getPortName() { return info.portName; }
    int getSerialNumber() { return info.serialNumberRegister; }
    batlabStatusInfo getInfo();

    void handleSerialResponseBundleReady(batlabPacketBundle bundle);
    void handleSerialPacketBundleSendFailed();
    void periodicCheck();
    bool hasReceivedValidResponse();
    void verifyBatlabDevice();
    void handleVerifyBatlabDeviceResponse(QQueue<BatlabPacket> response);
    void initBatlabDevice();
    void handleInitBatlabDeviceResponse(QQueue<BatlabPacket> response);


private slots:

private:
    batlabStatusInfo info;

    // TODO move to channels?
    int m_tempCalibB[4];
    int m_tempCalibR[4];

    QStateMachine batlabStateMachine;
    QState* s_unknown;
    QState* s_bootloader;
    QState* s_booted;

    BatlabCommsManager* m_commsManager;
};

#endif // BATLABCOM_H
