#include "batlabcommsmanager.h"

BatlabCommsManager::BatlabCommsManager(QString portName, QObject *parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(portName);
    m_serialPort->setBaudRate(DEFAULT_BAUD_RATE);
    m_serialWaiting = false;
    m_timer.setSingleShot(true);

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        emit error(tr("Can't open %1, error code %2.")
                .arg(portName).arg(m_serialPort->error()));
    }

     connect(m_serialPort, &QSerialPort::bytesWritten, this, &BatlabCommsManager::handleBytesWritten);
     connect(m_serialPort, &QSerialPort::errorOccurred, this, &BatlabCommsManager::handleError);
     connect(&m_timer, &QTimer::timeout, this, &BatlabCommsManager::handleTimeout);
}

void BatlabCommsManager::sendPacketBundle(batlabPacketBundle bundle)
{
    m_packetBundleQueue.append(bundle);
    processSerialQueue();
}

void BatlabCommsManager::processSerialQueue()
{
    if (m_serialWaiting) { return; }

    if (m_currentPacketBundle.packets.empty())
    {
        if (!m_packetBundleQueue.empty())
        {
            m_currentPacketBundle = m_packetBundleQueue.dequeue();
            processSerialQueue();
            // TODO handle when we get a success from the last packet in a packet bundle
        }
    }
    else
    {
        m_currentPacket = m_currentPacketBundle.packets.dequeue();
        QVector<uchar> request(5);
        request[0] = m_currentPacket.startByte;
        request[1] = m_currentPacket.nameSpace;
        request[2] = m_currentPacket.address;
        request[3] = m_currentPacket.payloadLowByte;
        request[4] = m_currentPacket.payloadHighByte;
        const qint64 bytesWritten = m_serialPort->write(reinterpret_cast<char*>(request.data()), 5);

        if (bytesWritten == -1)
        {
            emit error(tr("Failed to write data to port %1, error: %2")
                       .arg(m_serialPort->portName())
                       .arg(m_serialPort->errorString()));
            return;
        }
        else if (bytesWritten != 5)
        {
            emit error(tr("Failed to write all data to port %1, error: %2")
                       .arg(m_serialPort->portName())
                       .arg(m_serialPort->errorString()));
            return;
        }

        m_serialWaiting = true;
        m_timer.start(m_currentPacket.writeTimeout_ms);
    }
}

void BatlabCommsManager::handleBytesWritten(qint64 bytes)
{
    if (bytes != 5)
    {
        qWarning() << "Unable to write complete packet.";
    }

    // Start timer to wait for read response
    // Up front make sure readyRead is connected to something
    // LEFT OFF
    // TODO connect write timeout
}

void BatlabCommsManager::handleError(QSerialPort::SerialPortError serialPortError)
{
    // TODO
}

void BatlabCommsManager::handleTimeout()
{
    // TODO
}
