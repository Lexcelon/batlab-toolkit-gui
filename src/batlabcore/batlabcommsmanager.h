#ifndef BATLABCOMMSMANAGER_H
#define BATLABCOMMSMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

#include "batlablib.h"

class BatlabCommsManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabCommsManager(QString portName, QObject *parent = nullptr);

    void sendPacketBundle(batlabPacketBundle bundle);

signals:
    // TODO upstream handle these
    // TODO allow for retries before giving up
    void error(const QString &s);
    void responseBundleReady(batlabPacketBundle bundle);

public slots:

private:
    void processSerialQueue();
    void handleBytesWritten(qint64 bytes);
    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleTimeout();
    void handleReadyRead();
    void attemptWriteCurrentPacket();

    QSerialPort* m_serialPort;
    QQueue<batlabPacketBundle> m_packetBundleQueue;
    batlabPacketBundle m_currentPacketBundle;
    batlabPacket m_currentPacket;
    bool m_serialWaiting;
    QTimer m_timer;
    QByteArray m_readData;
    int m_retries;
};

#endif // BATLABCOMMSMANAGER_H
