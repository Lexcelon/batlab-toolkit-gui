#ifndef BATLABCOMMTHREAD_H
#define BATLABCOMMTHREAD_H

#include <chrono>
#include <thread>

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QQueue>

#include "batlablib.h"

struct batlabCommand {
    int waitTimeout;
    QVector<uchar> request;
    int sleepAfterTransaction;
};

class BatlabCommThread : public QThread
{
    Q_OBJECT

public:
    explicit BatlabCommThread(QObject *parent = nullptr);
    ~BatlabCommThread();

    void transaction(const int serialnumber, const QString &portName, int waitTimeout, const QVector<uchar> request, int sleepAfterTransaction);

signals:
    void response(const QVector<uchar> response);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

    QString m_portName;
    int m_serialnumber;

    QQueue<batlabCommand> m_commandQueue;

    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
};

#endif // BATLABCOMMTHREAD_H
