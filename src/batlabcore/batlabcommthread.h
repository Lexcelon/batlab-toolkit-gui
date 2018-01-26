#ifndef BATLABCOMMTHREAD_H
#define BATLABCOMMTHREAD_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPort>
#include <QThread>
#include <QTime>

class BatlabCommThread : public QThread
{
    Q_OBJECT

public:
    explicit BatlabCommThread(QString portName, QObject *parent = nullptr);
    ~BatlabCommThread();

    void transaction(int waitTimeout, const QString &request);

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

    QString m_portName;
    QString m_request;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
};

#endif // BATLABCOMMTHREAD_H
