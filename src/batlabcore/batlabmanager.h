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
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>

#include "batlab.h"
#include "cellplaylist.h"

// There is one BatlabManager for the entire program.
class BatlabManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabManager(QObject *parent = nullptr);
    QVector<batlabStatusInfo> getBatlabInfos();
    QVector<QString> getFirmwareVersions();
    int getLatestFirmwareVersion();

signals:
    void batlabInfoUpdated(QVector<batlabStatusInfo>, int latestFirmwareVersion);
    void cellPlaylistLoaded(CellPlaylist playlist);
    void cellPlaylistUpdated(CellPlaylist playlist);
    void cellResultsUpdated(QVector<cellResultsStatusInfo> infos);
    void error(QString e);
    void notify(QString title, QString text);

public slots:
    void updateConnectedBatlabs();
    void addNewBatlab(QString portName);
    void removeBatlab(QString portName);
    void processUpdatedBatlabInfo();
    void processRegisterReadRequest(int serial, int ns, int address);
    void processRegisterWriteRequest(int serial, int ns, int address, int value);
    void processFirmwareFlashRequest(int serial, QString firmwareVersion);
    void processFirmwareDownloadFinished();
    void processFirmwareDownloadError();
    void setAllBatlabChannelsIdle();

    void assignRemainingCellsToOpenChannels();
    void findBatlabForCell(cellResultsStatusInfo cell);

    void startTests();
    void stopTests();
    bool testsInProgress();

    void initializeNetworkAccessManager();
    void requestAvailableFirmwareVersions();
    void processAvailableFirmwareVersions();

    QString getPortNameFromSerial(int serial);

    void loadPlaylist(CellPlaylist playlist);
    bool hasPartialCellResults(CellPlaylist playlist);

private:
    bool isCellPlaylistLoaded;

    CellPlaylist loadedPlaylist;
    QMap<QString, cellResultsStatusInfo> m_cellResults;

    QMap<QString, Batlab*> candidateBatlabsByPortName;
    QMap<QString, Batlab*> connectedBatlabsByPortName;

    QMap<QString, QString> availableFirmwareVersionToUrl;
    QMap<int, QString> batlabSerialToFirmwareVersionWaiting;
    QMap<QString, QNetworkReply*> pendingFirmwareDownloadVersions;

    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* firmwareVersionsReply;
};

#endif // BATLABMANAGER_H
