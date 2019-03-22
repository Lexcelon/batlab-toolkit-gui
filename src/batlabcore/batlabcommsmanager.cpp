#include "batlabcommsmanager.h"
#include "batlab.h"

BatlabCommsManager::BatlabCommsManager(QString portName, QObject *parent)
    : QObject(parent) {
  m_serialPort = new QSerialPort(portName);
  m_serialPort->setBaudRate(DEFAULT_BAUD_RATE);
  m_serialWaiting = false;
  m_readWriteTimer.setSingleShot(true);
  m_sleepAfterTransactionTimer.setSingleShot(true);
  m_retries = 0;

  if (!m_serialPort->open(QIODevice::ReadWrite)) {
    qWarning() << tr("Can't open %1, error code %2.")
                      .arg(portName)
                      .arg(m_serialPort->error());
  }

  connect(m_serialPort, &QSerialPort::bytesWritten, this,
          &BatlabCommsManager::handleBytesWritten);
  connect(m_serialPort, &QSerialPort::errorOccurred, this,
          &BatlabCommsManager::handleError);
  connect(m_serialPort, &QSerialPort::readyRead, this,
          &BatlabCommsManager::handleReadyRead);
  connect(&m_readWriteTimer, &QTimer::timeout, this,
          &BatlabCommsManager::handleTimeout);
  connect(&m_sleepAfterTransactionTimer, &QTimer::timeout, this,
          &BatlabCommsManager::processSerialQueue);
}

void BatlabCommsManager::debug() {
  qDebug() << "\tPort Name: " << m_serialPort->portName()
           << "Waiting: " << m_serialWaiting
           << "\n\tBundles remaining in queue: " << m_packetBundleQueue.size()
           << "\n\tPackets remaining in current bundle: "
           << m_currentPacketBundle.packets.size()
           << "\n\tPackets in current response bundle: "
           << m_currentResponseBundle.packets.size()
           << "\n\tCurrent retry: " << m_retries;
}

BatlabCommsManager::~BatlabCommsManager() { delete m_serialPort; }

void BatlabCommsManager::sendPacketBundle(batlabPacketBundle bundle) {
  m_packetBundleQueue.append(bundle);
  processSerialQueue();
}

void BatlabCommsManager::processSerialQueue() {
  if (m_serialWaiting) {
    return;
  }

  // If we are done sending packets in current bundle
  if (m_currentPacketBundle.packets.empty()) {
    // If we have some response packets back, send the bundle
    if (!m_currentResponseBundle.packets.empty()) {
      // Make sure we got the expected number back
      if (m_currentResponseBundle.packets.size() == m_currentPacketBundleSize) {
        batlabPacketBundle emitBundle = m_currentResponseBundle;
        m_currentResponseBundle.packets.clear();
        emit responseBundleReady(emitBundle);
      } else {
        qWarning() << tr("Expected %1 response packets on port %2 but received "
                         "%3 instead")
                          .arg(m_currentPacketBundleSize)
                          .arg(m_serialPort->portName())
                          .arg(m_currentResponseBundle.packets.size());
        for (auto packet : m_currentResponseBundle.packets) {
          packet.debug();
        }
        fail();
      }
    }
    // If we have another bundle to deal with
    if (!m_packetBundleQueue.empty()) {
      m_currentPacketBundle = m_packetBundleQueue.dequeue();
      m_currentPacketBundleSize = m_currentPacketBundle.packets.size();
      m_currentResponseBundle.packets.clear();
      m_currentResponseBundle.callback = m_currentPacketBundle.callback;
      m_currentResponseBundle.channel = m_currentPacketBundle.channel;
      processSerialQueue();
    }
  } else {
    m_currentPacket = m_currentPacketBundle.packets.takeFirst();
    attemptWriteCurrentPacket();
  }
}

void BatlabCommsManager::fail() {
  batlabPacketBundle emitBundle = m_currentResponseBundle;
  m_currentPacketBundle.packets.clear();
  m_currentResponseBundle.packets.clear();
  m_serialWaiting = false;
  m_readWriteTimer.stop();
  m_sleepAfterTransactionTimer.stop();
  m_responseData.clear();
  m_retries = 0;
  emit packetBundleSendFailed(emitBundle);
}

void BatlabCommsManager::attemptWriteCurrentPacket() {
  if (m_retries++ >= DEFAULT_SERIAL_RETRIES) {
    fail();
    return;
  }

  // Patch for firmware < 3 ... current compensation bug in firmware, so moving
  // the control loop to software. We do this by now keeping track of the
  // setpoint in software, and then the control loop tweaks the firmware
  // setpoint
  if (m_currentPacket.getAddress() == CURRENT_SETPOINT &&
      m_currentPacket.getNamespace() < 4) {
    if (m_currentPacket.getValue() >
        575) { // Maximum value to write to this register
      m_currentPacket.setValue(575);
    }
    dynamic_cast<Batlab *>(parent())
        ->getChannel(m_currentPacket.getNamespace())
        ->setInternalSetpoint(575);
  }
  // Make sure we cant turn on the current compensation control loop in firmware
  if (m_currentPacket.getAddress() == SETTINGS &&
      m_currentPacket.getNamespace() == UNIT) {
    m_currentPacket.setValue(m_currentPacket.getValue() & ~0x0003);
  }

  QVector<uchar> request(5);
  request[0] = m_currentPacket.getStartByte();
  request[1] = m_currentPacket.getNamespace();
  request[2] = m_currentPacket.getAddress();
  request[3] = m_currentPacket.getPayloadLowByte();
  request[4] = m_currentPacket.getPayloadHighByte();

  if (!m_serialPort->isOpen()) {
    qWarning() << tr("Attempted packet write but serial port %1 is not open")
                      .arg(m_serialPort->portName());
    attemptWriteCurrentPacket();
    return;
  }
  const qint64 bytesWritten =
      m_serialPort->write(reinterpret_cast<char *>(request.data()), 5);

  if (bytesWritten == -1) {
    qWarning() << tr("Failed to write data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
    attemptWriteCurrentPacket();
    return;
  } else if (bytesWritten != 5) {
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
void BatlabCommsManager::handleBytesWritten(qint64 bytes) {
  if (bytes != 5) {
    qWarning() << tr("Failed to write all data to port %1, error: %2")
                      .arg(m_serialPort->portName())
                      .arg(m_serialPort->errorString());
    attemptWriteCurrentPacket();
    return;
  }

  // Start waiting for response
  m_readWriteTimer.start(m_currentPacket.getReadTimeout_ms());
}

void BatlabCommsManager::handleReadyRead() {
  m_responseData.append(m_serialPort->readAll());

  if (m_responseData.size() == 5) {
    m_readWriteTimer.stop();

    // Read the stuff
    BatlabPacket responsePacket = m_currentPacket;

    responsePacket.setStartByte(m_responseData[0]);
    responsePacket.setNamespace(m_responseData[1]);
    responsePacket.setAddress(m_responseData[2]);
    responsePacket.setPayloadLowByte(m_responseData[3]);
    responsePacket.setPayloadHighByte(m_responseData[4]);

    if (responsePacket.getStartByte() != m_currentPacket.getStartByte() ||
        responsePacket.getNamespace() != m_currentPacket.getNamespace() ||
        responsePacket.getAddress() != m_currentPacket.getAddress()) {
      qWarning() << tr("Response packet did not match command packet on "
                       "attempt %1 of %2.")
                        .arg(m_retries)
                        .arg(DEFAULT_SERIAL_RETRIES);
      responsePacket.debug();
      m_currentPacket.debug();
      attemptWriteCurrentPacket();
      return;
    }

    m_serialWaiting = false;
    m_responseData.clear();
    m_currentResponseBundle.packets.append(responsePacket);
    m_retries = 0;

    if (m_currentPacketBundle.callback == "handleFirmwareFlashResponse") {
      emit firmwareFlashProgress(m_currentPacketBundle.packets.size());
    }

    // Sleep until the next transaction (usually set to zero so won't sleep)
    m_sleepAfterTransactionTimer.start(
        m_currentPacket.getSleepAfterTransaction_ms());
  } else if (m_responseData.size() > 5) {
    m_responseData.clear();
  }
}

void BatlabCommsManager::handleError(
    QSerialPort::SerialPortError serialPortError) {
  m_responseData.clear();
  if (serialPortError == QSerialPort::WriteError) {
    qWarning()
        << tr("I/O error occurred while writing data to port %1, error: %2")
               .arg(m_serialPort->portName())
               .arg(m_serialPort->errorString());
    attemptWriteCurrentPacket();
  } else if (serialPortError == QSerialPort::ReadError) {
    qWarning()
        << tr("I/O error occurred while reading data from port %1, error: %2")
               .arg(m_serialPort->portName())
               .arg(m_serialPort->errorString());
    attemptWriteCurrentPacket();
  }
}

void BatlabCommsManager::handleTimeout() {
  m_responseData.clear();
  qWarning()
      << tr("Operation timed out for port %1 on attempt %2 of %3, error: %4")
             .arg(m_serialPort->portName())
             .arg(m_retries)
             .arg(DEFAULT_SERIAL_RETRIES)
             .arg(m_serialPort->errorString());
  attemptWriteCurrentPacket();
}
