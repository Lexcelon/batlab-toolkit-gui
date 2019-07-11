#ifndef BATLABMANAGER_H
#define BATLABMANAGER_H

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSslSocket>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QTimer>

#include "batlab.h"
#include "cellplaylist.h"

// There is one BatlabManager for the entire program.
class BatlabManager : public QObject {
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
  void testsInProgressState(bool testsInProgress);
  void cellPlaylistEditedState(bool edited);

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
  void processCellResultsUpdated();
  void abortAllTests();

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
  void updatePlaylist(CellPlaylist playlist);
  bool hasPartialCellResults(CellPlaylist playlist);
  CellPlaylist loadedPlaylist();

  void savePlaylist();
  void savePlaylistAs(QString filename);

private:
  bool m_isCellPlaylistLoaded;

  CellPlaylist m_loadedPlaylist;
  QMap<QString, cellResultsStatusInfo> m_cellResults;

  QMap<QString, Batlab *> candidateBatlabsByPortName;
  QMap<QString, Batlab *> connectedBatlabsByPortName;

  QMap<QString, QString> availableFirmwareVersionToUrl;
  QMap<int, QString> batlabSerialToFirmwareVersionWaiting;
  QMap<QString, QNetworkReply *> pendingFirmwareDownloadVersions;

  QNetworkAccessManager *networkAccessManager;
  QNetworkReply *firmwareVersionsReply;
};

#endif // BATLABMANAGER_H
