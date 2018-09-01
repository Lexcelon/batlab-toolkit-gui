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
    void error(const QString &s);

public slots:

private:
    void processSerialQueue();
    void handleBytesWritten(qint64 bytes);
    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleWriteTimeout();

    QSerialPort* m_serialPort;
    QQueue<batlabPacketBundle> m_packetBundleQueue;
    batlabPacketBundle m_currentPacketBundle;
    batlabPacket m_currentPacket;
    bool m_serialWaiting;
    QTimer m_timer;
};

#endif // BATLABCOMMSMANAGER_H
