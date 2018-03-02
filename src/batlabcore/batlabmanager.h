#ifndef BATLABMANAGER_H
#define BATLABMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "batlab.h"

// There is one BatlabManager for the entire program. It keeps track of connected Batlabs, manages their communication, and handles test state and execution.
class BatlabManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabManager(QObject *parent = nullptr);
    QVector<batlabDisplayInfo> getBatlabInfos();
    QVector<QString> getFirmwareVersions();

signals:
    void batlabInfoUpdated(QVector<batlabDisplayInfo>);

public slots:
    void updateConnectedBatlabs();
    void addNewBatlab(QString portName);
    void removeBatlab(QString portName);
    void processUpdatedBatlabInfo();
    void processRegisterReadRequest(int serial, int ns, int address);
    void processRegisterWriteRequest(int serial, int ns, int address, int value);
    void processFirmwareFlashRequest(int serial, QString firmwareVersion);
    void requestFirmwareFlash();

    void initializeNetworkAccessManager();
    void requestAvailableFirmwareVersions();
    void processAvailableFirmwareVersions();

    QString getPortNameFromSerial(int serial);

private:
    bool cellPlaylistLoaded;
    bool testsInProgress;

    QMap<QString, Batlab*> candidateBatlabsByPortName;
    QMap<QString, Batlab*> connectedBatlabsByPortName;

    QMap<QString, QString> availableFirmwareVersions;
    QMap<int, QString> batlabsWaitingForFirmwareFiles;

    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* firmwareVersionsReply;
    QNetworkReply* firmwareDownloadReply;
};

#endif // BATLABMANAGER_H
