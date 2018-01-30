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

// TODO remove comment this is getting called correctly
void BatlabCommThread::transaction(const QString &portName, int waitTimeout, const QVector<uchar> request)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;

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

        // Send request
        BatlabLib::debugCommandPacket(999, currentRequest);
        serial.write(reinterpret_cast<char*>(currentRequest.data()), 5);
        if (serial.waitForBytesWritten(currentWaitTimeout))
        {
            // Get response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                {
                    responseData += serial.readAll();
                }

                const QVector<uchar> response {
                    static_cast<uchar>(responseData[0]),
                    static_cast<uchar>(responseData[1]),
                    static_cast<uchar>(responseData[2]),
                    static_cast<uchar>(responseData[3]),
                    static_cast<uchar>(responseData[4]),
                };
                emit this->response(response);
            }
            else
            {
                emit timeout(tr("Wait read response timeout %1.")
                             .arg(QTime::currentTime().toString()));
            }
        }
        else
        {
            emit timeout(tr("Wait write request timeout %1.")
                         .arg(QTime::currentTime().toString()));

        }

        m_mutex.lock();

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

        currentCommand = m_commandQueue.dequeue();
        currentWaitTimeout = currentCommand.waitTimeout;
        currentRequest = currentCommand.request;

        m_mutex.unlock();
    }
}













































