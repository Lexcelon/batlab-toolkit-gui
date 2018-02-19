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

#include "batlab.h"

// There is one BatlabManager for the entire program. It keeps track of connected Batlabs, manages their communication, and handles test state and execution.
class BatlabManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabManager(QObject *parent = nullptr);
    QVector<batlabDisplayInfo> getBatlabInfos();

signals:
    void batlabInfoUpdated(QVector<batlabDisplayInfo>);

public slots:
    void updateConnectedBatlabs();
    void addNewBatlab(QString portName);
    void removeBatlab(QString portName);
    void processUpdatedBatlabInfo();
    void processRegisterReadRequest(int serial, int ns, int address);
    void processRegisterWriteRequest(int serial, int ns, int address, int value);

    void requestAvailableFirmwareVersions();
    void processAvailableFirmwareVersions();

private:
    bool cellPlaylistLoaded;
    bool testsInProgress;

    QMap<QString, Batlab*> candidateBatlabsByPortName;
    QMap<QString, Batlab*> connectedBatlabsByPortName;

    QVector<int> availableFirmwareVersions;

    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* networkReply;
};

#endif // BATLABMANAGER_H
