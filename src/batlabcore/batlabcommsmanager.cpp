#include "batlabcommsmanager.h"

BatlabCommsManager::BatlabCommsManager(QString portName, QObject *parent) : QObject(parent)
{
    m_serialPort = new QSerialPort(portName);
    m_serialPort->setBaudRate(DEFAULT_BAUD_RATE);
    m_serialWaiting = false;
    m_readWriteTimer.setSingleShot(true);
    m_sleepAfterTransactionTimer.setSingleShot(true);
    m_retries = 0;

    if (!m_serialPort->open(QIODevice::ReadWrite))
    {
        qWarning() << tr("Can't open %1, error code %2.")
                      .arg(portName).arg(m_serialPort->error());
    }

     connect(m_serialPort, &QSerialPort::bytesWritten, this, &BatlabCommsManager::handleBytesWritten);
     connect(m_serialPort, &QSerialPort::errorOccurred, this, &BatlabCommsManager::handleError);
     connect(m_serialPort, &QSerialPort::readyRead, this, &BatlabCommsManager::handleReadyRead);
     connect(&m_readWriteTimer, &QTimer::timeout, this, &BatlabCommsManager::handleTimeout);
     connect(&m_sleepAfterTransactionTimer, &QTimer::timeout, this, &BatlabCommsManager::processSerialQueue);
}

void BatlabCommsManager::debug()
{
    qDebug() << "\tPort Name: " << m_serialPort->portName() << "Waiting: " << m_serialWaiting
             << "\n\tBundles remaining in queue: " << m_packetBundleQueue.size()
             << "\n\tPackets remaining in current bundle: " << m_currentPacketBundle.packets.size()
             << "\n\tPackets in current response bundle: " << m_currentResponseBundle.packets.size()
             << "\n\tCurrent retry: " << m_retries;
}

void BatlabCommsManager::sendPacketBundle(batlabPacketBundle bundle)
{
    m_packetBundleQueue.append(bundle);
    processSerialQueue();
}

void BatlabCommsManager::processSerialQueue()
{
    if (m_serialWaiting) { return; }

    // If we are done with packets in current bundle
    if (m_currentPacketBundle.packets.empty())
    {
        // If we have some responses ready to go, send them
        if (!m_currentResponseBundle.packets.empty())
        {
            batlabPacketBundle emitBundle = m_currentResponseBundle;
            m_currentResponseBundle.packets.clear();
            emit responseBundleReady(emitBundle);
        }
        // If we have another bundle to deal with
        if (!m_packetBundleQueue.empty())
        {
            m_currentPacketBundle = m_packetBundleQueue.dequeue();
            m_currentResponseBundle.packets.clear();
            m_currentResponseBundle.callback = m_currentPacketBundle.callback;
            m_currentResponseBundle.channel = m_currentPacketBundle.channel;
            processSerialQueue();
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
    if (m_retries++ >= DEFAULT_SERIAL_RETRIES)
    {
        emit packetBundleSendFailed();
        return;
    }
    QVector<uchar> request(5);
    request[0] = m_currentPacket.getStartByte();
    request[1] = m_currentPacket.getNamespace();
    request[2] = m_currentPacket.getAddress();
    request[3] = m_currentPacket.getPayloadLowByte();
    request[4] = m_currentPacket.getPayloadHighByte();
    if (!m_serialPort->isOpen())
    {
        qWarning() << tr("Attempted packet write but serial port %1 is not open").arg(m_serialPort->portName());
        attemptWriteCurrentPacket();
        return;
    }
    const qint64 bytesWritten = m_serialPort->write(reinterpret_cast<char*>(request.data()), 5);

    if (bytesWritten == -1)
    {
        qWarning() << tr("Failed to write data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        attemptWriteCurrentPacket();
        return;
    }
    else if (bytesWritten != 5)
    {
        qWarning() << tr("Failed to write all data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        attemptWriteCurrentPacket();
        return;
    }

    m_serialWaiting = true;
    m_readWriteTimer.start(m_currentPacket.getWriteTimeout_ms());
}

// https://stackoverflow.com/questions/7512559/qt-qiodevicewrite-qtcpsocketwrite-and-bytes-written
void BatlabCommsManager::handleBytesWritten(qint64 bytes)
{
    if (bytes != 5)
    {
        qWarning() << tr("Failed to write all data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        attemptWriteCurrentPacket();
        return;
    }

    // Start waiting for response
    m_readWriteTimer.start(m_currentPacket.getReadTimeout_ms());
}

void BatlabCommsManager::handleReadyRead()
{
    m_responseData.append(m_serialPort->readAll());

    if (m_responseData.size() == 5)
    {
        m_readWriteTimer.stop();

        // Read the stuff
        BatlabPacket responsePacket = m_currentPacket;
        // TODO make a function to make these just a constructor
        responsePacket.setStartByte(m_responseData[0]);
        responsePacket.setNamespace(m_responseData[1]);
        responsePacket.setAddress(m_responseData[2]);
        responsePacket.setPayloadLowByte(m_responseData[3]);
        responsePacket.setPayloadHighByte(m_responseData[4]);

        if (responsePacket.getStartByte() != m_currentPacket.getStartByte()
                || responsePacket.getNamespace() != m_currentPacket.getNamespace()
                || responsePacket.getAddress() != m_currentPacket.getAddress())
        {
            qWarning() << tr("Response packet did not match command packet.");
            attemptWriteCurrentPacket();
            return;
        }

        // TODO handle read verify

        m_serialWaiting = false;
        m_responseData.clear();
        m_currentResponseBundle.packets.enqueue(responsePacket);
        m_retries = 0;

        // Sleep until the next transaction (usually set to zero so won't sleep)
        m_sleepAfterTransactionTimer.start(m_currentPacket.getSleepAfterTransaction_ms());
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
        attemptWriteCurrentPacket();
    }
    else if (serialPortError == QSerialPort::ReadError)
    {
        qWarning() << tr("I/O error occurred while reading data from port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
        attemptWriteCurrentPacket();
    }
}

void BatlabCommsManager::handleTimeout()
{
    m_responseData.clear();
    qWarning() << tr("Operation timed out for port %1, error: %2")
                  .arg(m_serialPort->portName())
                  .arg(m_serialPort->errorString());
    attemptWriteCurrentPacket();
}
