#include "batlabmanager.h"

BatlabManager::BatlabManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QVector<uchar>>("QVector<uchar>");

    cellPlaylistLoaded = false;
    testsInProgress = false;

    QTimer *updateConnectedBatlabsTimer = new QTimer(this);
    connect(updateConnectedBatlabsTimer, &QTimer::timeout, this, &BatlabManager::updateConnectedBatlabs);
    updateConnectedBatlabsTimer->start(200);

    networkAccessManager = nullptr;
    QTimer::singleShot(100, this, &BatlabManager::initializeNetworkAccessManager);
    QTimer::singleShot(500, this, &BatlabManager::requestAvailableFirmwareVersions);
}

void BatlabManager::initializeNetworkAccessManager()
{
    networkAccessManager = new QNetworkAccessManager;
}

void BatlabManager::requestAvailableFirmwareVersions()
{
    if (networkAccessManager != nullptr)
    {
        firmwareVersionsReply = networkAccessManager->get(QNetworkRequest(QUrl("https://api.github.com/repos/Lexcelon/batlab-firmware-measure/releases")));
        connect(firmwareVersionsReply, &QNetworkReply::finished, this, &BatlabManager::processAvailableFirmwareVersions);
    }
}

void BatlabManager::processAvailableFirmwareVersions()
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(firmwareVersionsReply->readAll());
    QJsonArray jsonArray = jsonDoc.array();
    for (int i = 0; i < jsonArray.size(); i++)
    {
        availableFirmwareVersionToUrl[jsonArray[i].toObject()["tag_name"].toString()] = jsonArray[i].toObject()["assets"].toArray()[0].toObject()["browser_download_url"].toString();
    }
}

void BatlabManager::updateConnectedBatlabs()
{
    // TODO make sure that things are actually Batlabs
    QList<QSerialPortInfo> availableCommPorts = QSerialPortInfo::availablePorts();
    QStringList availableCommPortNames;

    for (int i = 0; i < availableCommPorts.size(); i++) {
        // On Windows availableports() returns those that are not active, meaning you have to check individually
        // to see if they are active. Otherwise it never thinks you unplug a Batlab.
        // https://stackoverflow.com/questions/24854597/qserialportinfo-returns-more-com-ports-than-i-have
        // https://bugreports.qt.io/browse/QTBUG-39748
        if (!availableCommPorts.at(i).description().isEmpty()
                && availableCommPorts.at(i).vendorIdentifier() == 0x04D8
                && availableCommPorts.at(i).productIdentifier() == 0x000A)
        {
            availableCommPortNames.append(availableCommPorts.at(i).portName());
        }
    }

    // Check if Batlabs have been added
    for (int i = 0; i < availableCommPortNames.size(); i++) {
        QString portName = availableCommPortNames[i];
        if (!connectedBatlabsByPortName.contains(portName)) {
            addNewBatlab(portName);
        }
    }

    // Check if Batlabs have been removed
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        if (!availableCommPortNames.contains(portName)) {
            removeBatlab(portName);
        }
    }
}

void BatlabManager::addNewBatlab(QString portName)
{
    Batlab *batlab = new Batlab(portName);
    connect(batlab, &Batlab::infoUpdated, this, &BatlabManager::processUpdatedBatlabInfo);
    connectedBatlabsByPortName[portName] = batlab;
    processUpdatedBatlabInfo();
}

void BatlabManager::removeBatlab(QString portName)
{
    delete connectedBatlabsByPortName[portName];
    connectedBatlabsByPortName.remove(portName);
    processUpdatedBatlabInfo();
}

void BatlabManager::processUpdatedBatlabInfo()
{
    QVector<batlabStatusInfo> infos;
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        // Only show information for devices that have received valid responses (i.e. are Batlabs)
        if (connectedBatlabsByPortName[portName]->hasReceivedValidResponse())
        {
            batlabStatusInfo info = connectedBatlabsByPortName[portName]->getInfo();
            infos.push_back(info);
        }
    }
    emit batlabInfoUpdated(infos);
}

QVector<batlabStatusInfo> BatlabManager::getBatlabInfos()
{
    QVector<batlabStatusInfo> infos;
    for (int i = 0; i < connectedBatlabsByPortName.keys().size(); i++) {
        QString portName = connectedBatlabsByPortName.keys()[i];
        // Only return information for devices that have received valid responses (i.e. are Batlabs)
        if (connectedBatlabsByPortName[portName]->hasReceivedValidResponse())
        {
            batlabStatusInfo info = connectedBatlabsByPortName[portName]->getInfo();
            infos.push_back(info);
        }
    }
    return infos;
}

QVector<QString> BatlabManager::getFirmwareVersions()
{
    return availableFirmwareVersionToUrl.keys().toVector();
}

void BatlabManager::processRegisterReadRequest(int serial, int ns, int address)
{
    QString portName = getPortNameFromSerial(serial);
    if (!portName.isEmpty()) { connectedBatlabsByPortName[portName]->initiateRegisterRead(ns, address); }
}

void BatlabManager::processRegisterWriteRequest(int serial, int ns, int address, int value)
{
    QString portName = getPortNameFromSerial(serial);
    if (!portName.isEmpty()) { connectedBatlabsByPortName[portName]->initiateRegisterWrite(ns, address, value); }
}

void BatlabManager::processFirmwareFlashRequest(int serial, QString firmwareVersion)
{
    if (testsInProgress)
    {
        // TODO grey out firmware flash button when tests are in progress
        qWarning() << "Cannot flash firmware while tests are in progress.";
        return;
    }

    if (batlabSerialToFirmwareVersionWaiting.keys().contains(serial))
    {
        qWarning() << "Batlab " << QString::number(serial) << " already has pending firmware update.";
        // TODO replace that update with this one?
    }

    batlabSerialToFirmwareVersionWaiting[serial] = firmwareVersion;

    QString firmwareFileUrl = availableFirmwareVersionToUrl[firmwareVersion];
    QString firmwareFilename = QFileInfo(firmwareFileUrl).fileName();

    QDir appLocalDataPath(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).first());
    QString firmwareDirPath = appLocalDataPath.absoluteFilePath("firmware-bin-files");

    QDir dir;
    if (!dir.mkpath(firmwareDirPath)) {
        qWarning() << "Unable to find/make firmware download path.";
    }

    QString firmwareFilePath = QDir(firmwareDirPath).absoluteFilePath(firmwareFilename);
    QFileInfo firmwareFileInfo(firmwareFilePath);
    // Check if file already exists and is correct size. If so, flash.
    if (firmwareFileInfo.exists() && firmwareFileInfo.size() == FIRMWARE_FILE_SIZE)
    {
        batlabSerialToFirmwareVersionWaiting.remove(serial);
        connectedBatlabsByPortName[getPortNameFromSerial(serial)]->initiateFirmwareFlash(firmwareFilePath);
        return;
    }
    else
    {
        if (networkAccessManager == nullptr)
        {
            qWarning() << "Cannot request firmware download, network access manager has not been initialized.";
            return;
        }

        // Check if URL already in map
        // If yes, do nothing
        if (pendingFirmwareDownloadVersions.keys().contains(firmwareVersion))
        {
            // This firmware version already has a download pending. No action.
            return;
        }
        // If no, create network reply and add to map
        else
        {
            QNetworkRequest request = QNetworkRequest(QUrl(availableFirmwareVersionToUrl[firmwareVersion]));
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
            QNetworkReply* firmwareDownloadReply = networkAccessManager->get(request);
            pendingFirmwareDownloadVersions[firmwareVersion] = firmwareDownloadReply;

            connect(firmwareDownloadReply, &QNetworkReply::finished, this, &BatlabManager::processFirmwareDownloadFinished);
            connect(firmwareDownloadReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &BatlabManager::processFirmwareDownloadError);
        }
    }



            // connect timeout, error to function that removes it and throws warning
            // connect finished to function that processes waiting batlab list and kicks off flashing (and removes this from pending downloads)
}

void BatlabManager::processFirmwareDownloadError()
{
    QNetworkReply* firmwareDownloadReply = qobject_cast<QNetworkReply*>(QObject::sender());
    // TODO
}

void BatlabManager::processFirmwareDownloadFinished()
{
    // TODO Double check file is existent and correct size
    QNetworkReply* firmwareDownloadReply = qobject_cast<QNetworkReply*>(QObject::sender());

    // Find what firmware version this was
    QString firmwareVersion;
    for (auto version : pendingFirmwareDownloadVersions.keys())
    {
        if (pendingFirmwareDownloadVersions[version] == firmwareDownloadReply)
        {
            firmwareVersion = version;
        }
    }

    QString firmwareFileUrl = firmwareDownloadReply->url().toString();
    QString firmwareFilename = firmwareVersion + ".bin";

    QDir appLocalDataPath(QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).first());
    QString firmwareDirPath = appLocalDataPath.absoluteFilePath("firmware-bin-files");

    QDir dir;
    if (!dir.mkpath(firmwareDirPath)) {
        qWarning() << "Unable to find/make firmware download path.";
    }

    QString firmwareFilePath = QDir(firmwareDirPath).absoluteFilePath(firmwareFilename);
    QFile file(firmwareFilePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Could not open firmware file for write.";
        return;
    }
    const QByteArray data = firmwareDownloadReply->readAll();
    file.write(data);
    file.close();

    // TODO handle partially existent, already existent, no existent file

    // TODO For each Batlab wanting this, flash it
//    QString portName = getPortNameFromSerial(serial);
//    if (!portName.isEmpty()) { connectedBatlabsByPortName[portName]->initiateFirmwareFlash(firmwareFilePath); }
}

QString BatlabManager::getPortNameFromSerial(int serial)
{
    for (int i = 0; i < connectedBatlabsByPortName.size(); i++)
    {
        QString portName = connectedBatlabsByPortName.keys()[i];
        batlabStatusInfo info = connectedBatlabsByPortName[portName]->getInfo();
        if (info.serialNumberComplete == serial)
        {
            return portName;
        }
    }
    qWarning() << "No Batlab with serial" << serial << "was found.";
    return "";
}
