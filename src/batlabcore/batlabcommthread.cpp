#include "batlabcommthread.h"

BatlabCommThread::BatlabCommThread(QObject *parent) :
    QThread(parent)
{
}

BatlabCommThread::~BatlabCommThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void BatlabCommThread::transaction(const int serialnumber, const QString &portName, int waitTimeout, const QVector<uchar> request)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_serialnumber = serialnumber;

    // Add this command to this Batlab's queue.
    // Without the queue, this command wouldn't get sent if the thread were in the process of waiting for a response from another command.
    m_commandQueue.enqueue({waitTimeout, request});

    if (!isRunning())
    {
        start();
    }
    else
    {
        m_cond.wakeOne();
    }
}

void BatlabCommThread::run()
{
    bool currentPortNameChanged = false;

    m_mutex.lock();

    QString currentPortName;
    if (currentPortName != m_portName)
    {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }
    int currentSerialNumber = m_serialnumber;

    batlabCommand currentCommand = m_commandQueue.dequeue();
    int currentWaitTimeout = currentCommand.waitTimeout;
    QVector<uchar> currentRequest = currentCommand.request;

    m_mutex.unlock();

    QSerialPort serial;

    if (currentPortName.isEmpty())
    {
        emit error(tr("No port name specified."));
        return;
    }

    while (!m_quit)
    {
        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite))
            {
                emit error(tr("Can't open %1, error code %2.")
                        .arg(m_portName).arg(serial.error()));
                return;
            }
        }

        // Send command
        BatlabLib::debugCommandPacket(currentSerialNumber, currentRequest);
        serial.write(reinterpret_cast<char*>(currentRequest.data()), 5);
        if (serial.waitForBytesWritten(currentWaitTimeout))
        {
            // Get response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseDataBytes = serial.readAll();
                while (serial.waitForReadyRead(10))
                {
                    responseDataBytes += serial.readAll();
                }

                QVector<uchar> responseData;
                for (int i = 0; i < responseDataBytes.size(); i++)
                {
                    responseData.append(static_cast<uchar>(responseDataBytes[i]));
                }

                while (responseData.size() > 0)
                {
                    if (responseData[0] == 0xAA && responseData.size() >= 5)
                    {
                        if (responseData[1] != currentRequest[1] || responseData[2] != currentRequest[2])
                        {
                            qWarning() << "Received complete but unexpected command response from Batlab.";
                        }

                        QVector<uchar> response(5);
                        for (int i = 0; i < response.size(); i++)
                        {
                            response[i] = responseData[i];
                        }
                        responseData.remove(0, 5);

                        emit this->response(response);
                    }
                    else if (responseData[0] == 0xAF && responseData.size() >= 13)
                    {
                        // TODO handle stream
                    }
                    else
                    {
                        // Discard bytes until we get to a valid packet start byte
                        qWarning() << "Discarding unknown Batlab response data";
                        responseData.remove(0, 1);
                        continue;
                    }
                }
            }
            else
            {
                emit timeout(tr("Timeout waiting for response from Batlab %1.")
                             .arg(QTime::currentTime().toString()));
            }
        }
        else
        {
            emit timeout(tr("Timeout writing command to serial port %1.")
                         .arg(QTime::currentTime().toString()));

        }

        m_mutex.lock();

        // Continue sending commands and waiting for responses until we've worked through the queued commands.
        if (m_commandQueue.size() == 0)
        {
            m_cond.wait(&m_mutex);
        }

        if (currentPortName != m_portName)
        {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        }
        else
        {
            currentPortNameChanged = false;
        }
        currentSerialNumber = m_serialnumber;

        // Grab the next command and onward we go.
        currentCommand = m_commandQueue.dequeue();
        currentWaitTimeout = currentCommand.waitTimeout;
        currentRequest = currentCommand.request;

        m_mutex.unlock();
    }
}













































