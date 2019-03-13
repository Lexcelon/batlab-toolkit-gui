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
    Batlab(QString portName, QObject *parent = nullptr);
    ~Batlab();

    void setAllIdle();

signals:
    void batlabDisconnected(QString);
    void infoUpdated();

    void error(const QString &s);

public slots:
    void registerRead(int ns, int address);
    void handleRegisterReadResponse(QVector<BatlabPacket> response);
    void registerWrite(int ns, int address, int value);
    void handleRegisterWriteResponse(QVector<BatlabPacket> response);

    void initiateFirmwareFlash(QString firmwareFilePath);
    void handleFirmwareFlashResponse();
    void updateFirmwareFlashProgress(int packetsRemaining);

    QString getPortName() { return m_info.portName; }
    int getSerialNumber() { return m_info.serialNumberRegister; }
    bool getExternalPowerConnected() { return m_info.externalPowerConnected; }
    int getFirmwareVersion() { return m_info.firmwareVersion; }
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

private slots:

private:
    batlabStatusInfo m_info;
    Channel *m_channels[4];

    BatlabCommsManager* m_commsManager;
};

#endif // BATLABCOM_H
