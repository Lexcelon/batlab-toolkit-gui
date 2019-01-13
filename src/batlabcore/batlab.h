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
    void batlabDisconnected(QString);
    void infoUpdated();

    void error(const QString &s);

public slots:
    void initiateFirmwareFlash(QString firmwareFilePath);
    void handleFirmwareFlashResponse(QVector<BatlabPacket> response);
    void updateFirmwareFlashProgress(int packetsRemaining);

    QString getPortName() { return m_info.portName; }
    int getSerialNumber() { return m_info.serialNumberRegister; }
    batlabStatusInfo getInfo();

    void handleSerialResponseBundleReady(batlabPacketBundle bundle);
    void handleSerialPacketBundleSendFailed(batlabPacketBundle bundle);

    void periodicCheck();
    void handlePeriodicCheckResponse(QVector<BatlabPacket> response);

    bool hasReceivedValidResponse();

    void verifyBatlabDevice();
    void handleVerifyBatlabDeviceResponse(QVector<BatlabPacket> response);

    void initBatlabDevice();
    void handleInitBatlabDeviceResponse(QVector<BatlabPacket> response);

    void setWatchdogTimer();
    void handleSetWatchdogTimerResponse(QVector<BatlabPacket> response);

private slots:

private:
    batlabStatusInfo m_info;

    // TODO move to channels?
    int m_tempCalibB[4];
    int m_tempCalibR[4];

    BatlabCommsManager* m_commsManager;
};

#endif // BATLABCOM_H
