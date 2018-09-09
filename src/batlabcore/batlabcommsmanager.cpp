#include "batlabcommsmanager.h"

BatlabCommsManager::BatlabCommsManager(QString portName, QObject *parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(portName);
    m_serialPort->setBaudRate(DEFAULT_BAUD_RATE);
    m_serialWaiting = false;
    m_timer.setSingleShot(true);
    m_retries = 0;

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        qWarning() << tr("Can't open %1, error code %2.")
                      .arg(portName).arg(m_serialPort->error());
    }

     connect(m_serialPort, &QSerialPort::bytesWritten, this, &BatlabCommsManager::handleBytesWritten);
     connect(m_serialPort, &QSerialPort::errorOccurred, this, &BatlabCommsManager::handleError);
     connect(m_serialPort, &QSerialPort::readyRead, this, &BatlabCommsManager::handleReadyRead);
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
        if (!m_currentResponseBundle.packets.empty())
        {
            emit responseBundleReady(m_currentResponseBundle);
        }
        if (!m_packetBundleQueue.empty())
        {
            m_currentPacketBundle = m_packetBundleQueue.dequeue();
            m_currentResponseBundle.packets.clear();
            m_currentResponseBundle.callback = m_currentPacketBundle.callback;
            m_currentResponseBundle.channel = m_currentPacketBundle.channel;
            processSerialQueue();
            // TODO handle when we get a success from the last packet in a packet bundle
        }
    }
    else
    {
        m_currentPacket = m_currentPacketBundle.packets.dequeue();
        attemptWriteCurrentPacket();
    }
}

void BatlabCommsManager::attemptWriteCurrentPacket()
{
    // TODO check retries
    QVector<uchar> request(5);
    request[0] = m_currentPacket.startByte;
    request[1] = m_currentPacket.nameSpace;
    request[2] = m_currentPacket.address;
    request[3] = m_currentPacket.payloadLowByte;
    request[4] = m_currentPacket.payloadHighByte;
    const qint64 bytesWritten = m_serialPort->write(reinterpret_cast<char*>(request.data()), 5);

    if (bytesWritten == -1)
    {
        qWarning() << tr("Failed to write data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        return;
    }
    else if (bytesWritten != 5)
    {
        qWarning() << tr("Failed to write all data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        return;
    }

    m_serialWaiting = true;
    m_timer.start(m_currentPacket.writeTimeout_ms);
}

// https://stackoverflow.com/questions/7512559/qt-qiodevicewrite-qtcpsocketwrite-and-bytes-written
void BatlabCommsManager::handleBytesWritten(qint64 bytes)
{
    if (bytes != 5)
    {
        qWarning() << tr("Failed to write all data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        return;
    }

    // Start waiting for response
    m_timer.start(m_currentPacket.readTimeout_ms);
}

void BatlabCommsManager::handleReadyRead()
{
    m_responseData.append(m_serialPort->readAll());

    if (m_responseData.size() == 5)
    {
        m_timer.stop();

        // Read the stuff
        batlabPacket responsePacket = m_currentPacket;
        responsePacket.startByte = m_responseData[0];
        responsePacket.nameSpace = m_responseData[1];
        responsePacket.address = m_responseData[2];
        responsePacket.payloadLowByte = m_responseData[3];
        responsePacket.payloadHighByte = m_responseData[4];

        if (responsePacket.startByte != m_currentPacket.startByte
                || responsePacket.nameSpace != m_currentPacket.nameSpace
                || responsePacket.address != m_currentPacket.address)
        {
            qWarning() << tr("Response packet did not match command packet.");
            return;
        }

        // TODO sleep if requested

        m_serialWaiting = false;
        m_responseData.clear();
        m_currentResponseBundle.packets.enqueue(responsePacket);
        processSerialQueue();
    }
}

void BatlabCommsManager::handleError(QSerialPort::SerialPortError serialPortError)
{
    m_responseData.clear();
    if (serialPortError == QSerialPort::WriteError)
    {
        qWarning() << tr("I/O error occurred while writing data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
    }
    else if (serialPortError == QSerialPort::ReadError)
    {
        qWarning() << tr("I/O error occurred while reading data from port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
    }
}

void BatlabCommsManager::handleTimeout()
{
    m_responseData.clear();
    qWarning() << tr("Operation timed out for port %1, error: %2")
                  .arg(m_serialPort->portName())
                  .arg(m_serialPort->errorString());
}
