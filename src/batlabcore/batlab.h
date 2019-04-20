#ifndef BATLABCOM_H
#define BATLABCOM_H

#include <QComboBox>
#include <QDebug>
#include <QFileInfo>
#include <QInputDialog>
#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QVector>
#include <qmath.h>

#include "batlabcommsmanager.h"
#include "batlablib.h"
#include "cellplaylist.h"
#include "channel.h"

class Batlab : public QObject {
  Q_OBJECT
public:
  Batlab(QString portName, QObject *parent = nullptr);
  ~Batlab();

  void setAllIdle();
  bool testsInProgress();

signals:
  void batlabDisconnected(QString);
  void infoUpdated();
  void cellResultsUpdated();

  void error(const QString &s);

public slots:
  void sendPacketBundle(batlabPacketBundle bundle);

  void registerRead(int ns, int address);
  void handleRegisterReadResponse(QVector<BatlabPacket> response);
  void registerWrite(int ns, int address, int value);
  void handleRegisterWriteResponse(QVector<BatlabPacket> response);

  void initiateFirmwareFlash(QString firmwareFilePath);
  void handleFirmwareFlashResponse();
  void updateFirmwareFlashProgress(int packetsRemaining);

  QString getPortName() { return m_info.portName; }
  int getSerialNumber() { return m_info.serialNumberComplete; }
  bool getExternalPowerConnected() { return m_info.externalPowerConnected; }
  int getFirmwareVersion() { return m_info.firmwareVersion; }
  batlabStatusInfo getInfo();

  void handleSerialResponseBundleReady(batlabPacketBundle bundle);
  void handleSerialPacketBundleSendFailed(batlabPacketBundle bundle);

  void periodicCheck();
  void handlePeriodicCheckResponse(QVector<BatlabPacket> response);

  bool hasReceivedValidResponse();
  bool inBootloader();

  void verifyBatlabDevice();
  void handleVerifyBatlabDeviceResponse(QVector<BatlabPacket> response);

  void initBatlabDevice();
  void handleInitBatlabDeviceResponse(QVector<BatlabPacket> response);

  void setWatchdogTimer();

  Channel *getChannel(int slot);

  CellPlaylist playlist();

private slots:

private:
  batlabStatusInfo m_info;
  Channel *m_channels[4];

  BatlabCommsManager *m_commsManager;
};

#endif // BATLABCOM_H
