#include "batlabcommsmanager.h"

BatlabCommsManager::BatlabCommsManager(QString portName, QObject *parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(portName);
    m_serialPort->setBaudRate(DEFAULT_BAUD_RATE);
    m_serialWaiting = false;

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        emit error(tr("Can't open %1, error code %2.")
                .arg(portName).arg(m_serialPort->error()));
    }

     connect(m_serialPort, &QSerialPort::bytesWritten, this, &BatlabCommsManager::handleBytesWritten);
     connect(m_serialPort, &QSerialPort::errorOccurred, this, &BatlabCommsManager::handleError);
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
        m_serialPort->write(reinterpret_cast<char*>(request.data()), 5);

        m_serialWaiting = true;
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

void BatlabCommsManager::handleWriteTimeout()
{
    // TODO
}
