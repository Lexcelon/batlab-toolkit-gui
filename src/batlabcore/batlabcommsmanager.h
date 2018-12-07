#ifndef BATLABCOMMSMANAGER_H
#define BATLABCOMMSMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

#include "batlablib.h"
#include "batlabpacket.h"

class BatlabCommsManager : public QObject
{
    Q_OBJECT
public:
    explicit BatlabCommsManager(QString portName, QObject *parent = nullptr);
    ~BatlabCommsManager();

    void sendPacketBundle(batlabPacketBundle bundle);

signals:
    void packetBundleSendFailed(batlabPacketBundle bundle);
    void responseBundleReady(batlabPacketBundle bundle);

public slots:

private:
    void processSerialQueue();
    void handleBytesWritten(qint64 bytes);
    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleTimeout();
    void handleReadyRead();
    void attemptWriteCurrentPacket();
    void fail();

    void debug();

    QSerialPort* m_serialPort;
    QQueue<batlabPacketBundle> m_packetBundleQueue;
    batlabPacketBundle m_currentPacketBundle;
    int m_currentPacketBundleSize;
    batlabPacketBundle m_currentResponseBundle;
    BatlabPacket m_currentPacket;
    bool m_serialWaiting;
    QTimer m_readWriteTimer;
    QTimer m_sleepAfterTransactionTimer;
    QByteArray m_responseData;
    int m_retries;
};

#endif // BATLABCOMMSMANAGER_H
